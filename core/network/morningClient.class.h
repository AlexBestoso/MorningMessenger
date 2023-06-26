class MorningClient{
	private:
		NetSnake netSnake;
                FileSnake fileSnake;
                EncryptionSnake encryptionSnake;
		EncryptionSnake remoteCerts;
                MorningConfig config;
		MorningIO io;
		MorningServer server;

                string cmd_newUser = "a235210cd5476dfa0a045d60244e5cc6aebbc21e406fba344ce5d154b6337f5b";
                string cmd_existingUser = "f0773273589a87e67e71f402c08c93b464ff307846de3e7567dc1e423d65baf9";
                string cmd_streamChat = "6c0df3e03ccc8a63b41937acc7169cabad337da6bc7bb36a19245548d6eef2a3";
                string cmd_chat = "7b5a3431691d02a5f335235bae96e65301311115d3688e2306c0a664ab29bfbe";

		string serverPublicKey = "";

		size_t rsaBufSize = 1024;
		size_t ctrBufSize = 5000;

		size_t levelOneCommandCount = 3;
		string levelOneCommands[3] = {
			"request_new",
			"connect",
			"quit"
		};

		int parseSelectedOption(string input, string *options, size_t len){
                        for(int i=0; i<len; i++){
                                if(input == options[i]){
                                        return i+1;
                                }
                        }

                        int inputInt = atoi(input.c_str());
                        for(int i=0; i<len; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }

                        return -1;
                }

		bool validateResponse(){
			char *okay = new char[2];
                         if(!netSnake.recvInetClient(okay, 2, 0)){
                         	netSnake.closeSocket();
                                io.out(MORNING_IO_ERROR, "Failed to recv command validation\n");
                                return false;
                         }
                         string okayString = "";
			 for(int i=0; i<netSnake.recvSize; i++)
				 okayString += okay[i];
			 if(okayString == "OK"){
			 	return true;
			 }
			 return false;
		}

		__attribute__((deprecated("Function is being removed.")))int getLevelOne(void){
			while(1){
				for(int i=0; i<levelOneCommandCount; i++){
					io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, levelOneCommands[i].c_str());
				}
				string input = io.inString(MORNING_IO_INPUT, " > ");
				int option = parseSelectedOption(input, levelOneCommands, levelOneCommandCount);
				if(option == -1){
					io.out(MORNING_IO_ERROR, "Invalid option.\n");
				}else if(option == 1){
					return 1;
				}else if(option == 2){
					return 2;
				}else if(option == 3){
					return 3;
				}else{
					return -1;
				}
			}
			return -1;
		}

		bool rsaSend(string msg, size_t msgLen){
			if(msgLen > 1024){
				msgLen = 1024;
			}
                        string encryptedMessage = encryptionSnake.rsa(true, msg, msgLen);
                        if(encryptionSnake.didFail()){
                                return false;
                        }
                        size_t encryptedMessageLen = encryptionSnake.getResultLen();

                        if(!netSnake.sendInetClient(encryptedMessage.c_str(), encryptedMessageLen)){
                                return false;
                        }

                        return true;
                }

                string rsaRecv(void){
                        string ret = "";
                        char *buf = new char[rsaBufSize];
                        if(!netSnake.recvInetClient(buf, rsaBufSize, 0)){
                                return "";
                        }
                        size_t recvSize = netSnake.recvSize;
                        for(int i=0; i<recvSize; i++)
                                ret += buf[i];

                        ret = encryptionSnake.rsa(false, ret, recvSize);
                        if(encryptionSnake.didFail()){
				encryptionSnake.printError();
                                return "";
                        }
                        return ret;
                }

		void ctrSend(string msg, size_t msgLen){
                        string encryptedMsg = encryptionSnake.aes256ctr_execute(true, msg, msgLen);
                        if(encryptionSnake.didFail()){
				netSnake.closeConnection();
				throw MorningException("ctrSend: Failed to encrypt message sent to server.");
                        }

                        if(!netSnake.sendInetClient(encryptedMsg.c_str(), encryptionSnake.getResultLen())){
				netSnake.closeConnection();
				throw MorningException("ctrSend: Failed to send encrypted message to server.");
                        }
                }
                string ctrRecv(void){
                        string ret = "";
                        char *recvBuffer = new char[ctrBufSize];
                        if(!netSnake.recvInetClient(recvBuffer, ctrBufSize, 0)){
                                delete[] recvBuffer;
				netSnake.closeConnection();
                                throw MorningException("ctrRecv: Failed to receive message from server.");
                        }
                        for(int i=0; i<netSnake.recvSize; i++)
                                ret += recvBuffer[i];
                        delete[] recvBuffer;

                        ret = encryptionSnake.aes256ctr_execute(false, ret, netSnake.recvSize);
                        if(encryptionSnake.didFail()){
                         	netSnake.closeConnection();
			 	throw MorningException("ctrRecv: Failed to decipher received message from server.");
			}

                        return ret;
                }

		void sendPublicKey(){
			server.loadConfigs();
			serverconfig_t cfg = server.getServerConfig();
                        string key = cfg.publicKey;

                        if(!netSnake.sendInetClient(key.c_str(), key.length())){
                                netSnake.closeSocket();
                                throw MorningException("sendPublicKey: Failed to send public key to target server.");
                        }

                        if(netSnake.sendSize != key.length()){
                                netSnake.closeSocket();
                                throw MorningException("Failed to send the server your entire key.\n");
                        }
                }

		void recvPublicKey(){
                        size_t keySize = 1466;
                        string key = "";
                        char buffer[1466];

                        serverPublicKey = "";
                        if(!netSnake.recvInetClient(buffer, keySize, 0)){
                                netSnake.closeSocket();
				throw MorningException("recvPublicKey: Failed to receive public key from server.");
                        }

                        for(int i=0; i<netSnake.recvSize; i++)
                                serverPublicKey += buffer[i];

                        // Ensure full key is received.
                        int remaining = keySize - netSnake.recvSize;
                        if(remaining > 0){
                                memset(buffer, 0x00, keySize);
                                if(!netSnake.recvInetClient(buffer, remaining, 0)){
                                        netSnake.closeSocket();
                                        throw MorningException("recvPublicKey: Failed to receive the remainder of the server's public key.");
                                }

                                for(int i=0; i<netSnake.recvSize; i++)
                                        serverPublicKey += buffer[i];
                                remaining = remaining - netSnake.recvSize;
                        }


                        encryptionSnake.cleanOutPublicKey();
                        encryptionSnake.fetchRsaKeyFromString(false, false, serverPublicKey.c_str(), serverPublicKey.length(), "");
                        if(encryptionSnake.didFail()){
                                netSnake.closeSocket();
                                serverPublicKey = "";
                                throw MorningException("recvPublicKey: Failed to parse received RSA public key.\n");
                        }
                }

		void recvCtrKey(){
                        encryptionSnake.aes256ctr_stop(true);
                        encryptionSnake.aes256ctr_stop(false);

                        string keyPacket = rsaRecv();
                        if(keyPacket == "" || encryptionSnake.getResultLen() != 32+16)
                                throw MorningException("recvCtrKey: Failed to receive RSA encrypted message from server.");

                        unsigned char iv[16];
                        for(int i=0; i<16; i++)
                                iv[i] = keyPacket[i];

                        unsigned char key[32];
                        for(int i=0; i<32; i++)
                                key[i] = keyPacket[16+i];

                        if(!encryptionSnake.aes256ctr_start(true, key, iv))
                                throw MorningException("recvCtrKey: Failed to load received key and iv for encryption.");

                        if(!encryptionSnake.aes256ctr_start(false, key, iv))
                                throw MorningException("recvCtrKey: Failed to load received key and iv for decryption.");
                }

		void keyExchange(void){
			try{
				sendPublicKey();
				recvPublicKey();
				recvCtrKey();
			}catch(exception &e){
				netSnake.closeSocket();
				string err = e.what();
				err = "keyExchange: Failed to process key exchange.\n" + err;
				throw MorningException(err);
			}
		}


		void connectClient(string host, int port){
			if(!netSnake.createClient(host, port, 0))
				throw MorningException("connectClient: Failed to create client\nNetSnake::createClient : %s\n", netSnake.errorMessage().c_str());
		}
		void sendAccessRequest(){
			if(!netSnake.sendInetClient(cmd_newUser.c_str(), cmd_newUser.length())){
				netSnake.closeSocket();
				throw MorningException("sendAccessRequest : Failed to send command to remote server.\n");
			}

			if(!validateResponse()){
				netSnake.closeSocket();
				throw MorningException("sendAccessRequest : Failed to validate command response.\n");
			}
		}

		bool sendAuthRequest(){
			if(!netSnake.sendInetClient(cmd_existingUser.c_str(), cmd_existingUser.length())){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to send command to remote server\n");
				return false;
			}

			if(!validateResponse()){
				netSnake.closeSocket();
				return false;
			}
			return true;
		}

	public:

		void setConfig(MorningConfig config){
                	this->config = config;
        	}
        	void setEncryptionSnake(EncryptionSnake encryptionSnake){
        	        this->encryptionSnake = encryptionSnake;
        	}
		
		
		bool requestAccess(string host, int port){
			try{
				server.loadConfigs();
				serverconfig_t cfg = server.getServerConfig();
				encryptionSnake.cleanOutPrivateKey();
				encryptionSnake.cleanOutPublicKey();
				encryptionSnake.fetchRsaKeyFromString(true, false, cfg.privateKey, cfg.privateKey.length(), cfg.keyPassword);
				if(encryptionSnake.didFail())
					throw MorningException("Failed to load client private key.\n");
				encryptionSnake.fetchRsaKeyFromString(false, false, cfg.publicKey, cfg.publicKey.length(), "");
                                if(encryptionSnake.didFail())
                                        throw MorningException("Failed to load client public key.\n");

				this->connectClient(host, port);
				this->sendAccessRequest();
				this->keyExchange();

                                string msg = this->ctrRecv();
                                string name = io.inWithSpace(MORNING_IO_NONE, msg);
                                this->ctrSend(name, name.length());

				msg = ctrRecv();
                                string reason = io.inWithSpace(MORNING_IO_NONE, msg);
                                this->ctrSend(reason, reason.length());
				io.out(MORNING_IO_GENERAL, "Processing request...\n");
				sleep(1);
				this->ctrSend(cfg.serverHost, cfg.serverHost.length());
				sleep(1);
				this->ctrSend(to_string(cfg.serverPort), to_string(cfg.serverPort).length());
				sleep(1);
				this->ctrSend(cfg.serverName, cfg.serverName.length());

				// check with server to see if successful.
				msg = ctrRecv();

                                netSnake.closeSocket();
                                if(msg != "OK")
					throw MorningException("Server failed to process your request.");
			}catch(exception &e){
				io.outf(MORNING_IO_ERROR, "requestAccess : %s\n", e.what());
				return false;
			}
			return true;
		}

		string sendMessage(string ip, int port, string message){
			server.loadConfigs();
                        serverconfig_t cfg = server.getServerConfig();
                        encryptionSnake.cleanOutPrivateKey();
                        encryptionSnake.cleanOutPublicKey();
                        encryptionSnake.fetchRsaKeyFromString(true, false, cfg.privateKey, cfg.privateKey.length(), cfg.keyPassword);
                        if(encryptionSnake.didFail())
                                throw MorningException("Failed to load client private key.\n");
                        encryptionSnake.fetchRsaKeyFromString(false, false, cfg.publicKey, cfg.publicKey.length(), "");
                        if(encryptionSnake.didFail())
                                throw MorningException("Failed to load client public key.\n");

			if(!netSnake.createClient(ip, port, 0)){
                        	return "";
                        }
			sendAuthRequest();
			keyExchange();
		
			ctrSend(message, message.length());

			string response = ctrRecv();
			netSnake.closeSocket();
			return response;
		}
		__attribute__((deprecated("This function is being replaced")))bool connectToServer(void){
			string ip = io.inString(MORNING_IO_INPUT, "Enter Host Name > ");
			int port = atoi(io.inString(MORNING_IO_INPUT, "Enter Port Number (Default : 21345) > ").c_str());
			
			int levelOne = getLevelOne();
			if(levelOne == -1){
				return false;
			}else if(levelOne == 1){ // Access Request Protocol

			}else if(levelOne == 2){ // Try to authenticate with remote server
				if(!netSnake.createClient(ip, port, 0)){
					return false;
				}
				sendAuthRequest();
				keyExchange();

				string outMsg = io.inWithSpace(MORNING_IO_NONE, "Enter your message > ");
				ctrSend(outMsg, outMsg.length());

				string inMsg = ctrRecv();

				io.outf(MORNING_IO_NONE, "%s\n", inMsg.c_str());
				netSnake.closeSocket();
				return true;
			}else if(levelOne == 3){
				return true;
			}else{
				io.out(MORNING_IO_ERROR, "Invalid command attempted.\n");
				return false;
			}
			return true;
		}
};
