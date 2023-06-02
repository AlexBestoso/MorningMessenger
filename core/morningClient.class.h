class MorningClient{
	private:
		NetSnake netSnake;
                FileSnake fileSnake;
                EncryptionSnake encryptionSnake;
		EncryptionSnake remoteCerts;
                MorningConfig config;
		MorningIO io;

                string cmd_newUser = "a235210cd5476dfa0a045d60244e5cc6aebbc21e406fba344ce5d154b6337f5b";
                string cmd_existingUser = "f0773273589a87e67e71f402c08c93b464ff307846de3e7567dc1e423d65baf9";
                string cmd_streamChat = "6c0df3e03ccc8a63b41937acc7169cabad337da6bc7bb36a19245548d6eef2a3";
                string cmd_chat = "7b5a3431691d02a5f335235bae96e65301311115d3688e2306c0a664ab29bfbe";

		string serverPublicKey = "";

		size_t rsaBufSize = 1024;
		size_t ctrBufSize = 5000;

		size_t levelOneCommandCount = 2;
		string levelOneCommands[2] = {
			"request_new",
			"connect"
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

		int getLevelOne(void){
			while(1){
				for(int i=0; i<levelOneCommandCount; i++){
					printf("%d) %s\n", i+1, levelOneCommands[i].c_str());
				}
				string input = io.inString(MORNING_IO_INPUT, " > ");
				int option = parseSelectedOption(input, levelOneCommands, levelOneCommandCount);
				if(option == -1){
					io.out(MORNING_IO_ERROR, "Invalid option.\n");
				}else if(option == 1){
					return 1;
				}else if(option == 2){
					return 2;
				}else{
					return -1;
				}
			}
			return -1;
		}

		bool sendPublicKey(){
			mornconf cfg = config.getConfig();
			size_t size = fileSnake.getFileSize(cfg.pubkey);
			string keySize = to_string(size);
			if(!netSnake.sendInetClient((char *)keySize.c_str(), keySize.length())){
				netSnake.closeSocket();
				return false;
			}

			char *buffer = new char[size];
			if(!fileSnake.readFile(cfg.pubkey, buffer, size)){
				netSnake.closeSocket();
				delete[] buffer;
				return false;
			}

			if(!netSnake.sendInetClient(buffer, size)){
				netSnake.closeSocket();
				delete[] buffer;
				return false;
			}
			delete[] buffer;
			return true;
		}

		bool recvPublicKey(){
			size_t keySize = 0;
                        string key = "";
                        char *buffer = new char[6];
                        // recveive key size
                        if(!netSnake.recvInetClient(buffer, 6, 0)){
                                netSnake.closeSocket();
                                delete[] buffer;
                                return false;
                        }

                        keySize = atoi(buffer);
                        delete[] buffer;
                        if(keySize <= 0){
                                netSnake.closeSocket();
                                return false;
                        }
                        buffer = new char[keySize];
                        if(!netSnake.recvInetClient(buffer, keySize, 0)){
                                netSnake.closeSocket();
                                return false;
                        }

                        serverPublicKey = "";
                        for(int i=0; i<netSnake.recvSize; i++)
                                serverPublicKey += buffer[i];
                        delete[] buffer;

			encryptionSnake.cleanOutPublicKey();
                        encryptionSnake.fetchRsaKeyFromString(false, false, serverPublicKey.c_str(), netSnake.recvSize, "");
                        if(encryptionSnake.didFail()){
                                netSnake.closeSocket();
                                serverPublicKey = "";
                                return false;
                        }
                    
			return true;
		}

		bool recvCtrKey(){
			encryptionSnake.aes256ctr_stop(true);
			encryptionSnake.aes256ctr_stop(false);
			
			string keyPacket = rsaRecv();
			if(keyPacket == "" || encryptionSnake.getResultLen() != 32+16){
				return false;
			}

			unsigned char key[32];
			unsigned char iv[16];
			for(int i=0; i<16; i++){
				iv[i] = keyPacket[i];
			}
			for(int i=0; i<32; i++){
				key[i] = keyPacket[16+i];
			}

			if(!encryptionSnake.aes256ctr_start(true, key, iv)){
                                return false;
                        }

                        if(!encryptionSnake.aes256ctr_start(false, key, iv)){
                                return false;
                        }
			return true;
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

		bool ctrSend(string msg, size_t msgLen){
                        string encryptedMsg = encryptionSnake.aes256ctr_execute(true, msg, msgLen);
                        if(encryptionSnake.didFail()){
                                return false;
                        }

                        if(!netSnake.sendInetClient(encryptedMsg.c_str(), encryptionSnake.getResultLen())){
                                return false;
                        }
                        return true;
                }
                string ctrRecv(void){
                        string ret = "";
                        char *recvBuffer = new char[ctrBufSize];
                        if(!netSnake.recvInetClient(recvBuffer, ctrBufSize, 0)){
                                delete[] recvBuffer;
                                return "";
                        }
                        for(int i=0; i<netSnake.recvSize; i++){
                                ret += recvBuffer[i];
                        }
                        delete[] recvBuffer;

                        ret = encryptionSnake.aes256ctr_execute(false, ret, netSnake.recvSize);
                        if(encryptionSnake.didFail()){
                                return "";
                        }

                        return ret;
                }

		bool keyExchange(void){
			if(!sendPublicKey()){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to send public key.\n");
				return false;
			}

			if(!recvPublicKey()){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to recv public key\n");
				return false;
			}

			if(!recvCtrKey()){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to receive CTR key\n");
				return false;
			}
			return true;
		}

		bool sendAccessRequest(){
			if(!netSnake.sendInetClient(cmd_newUser.c_str(), cmd_newUser.length())){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to send command to remote server\n");
				return false;
			}

			if(!validateResponse()){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to validate command response.\n");
				return false;
			}
			return true;
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
		
		bool connectToServer(void){
			string ip = io.inString(MORNING_IO_INPUT, "Enter Host Name > ");
			int port = atoi(io.inString(MORNING_IO_INPUT, "Enter Port Number > ").c_str());
			if(!netSnake.createClient(ip, port, 0)){
				return false;
			}
			
			int levelOne = getLevelOne();
			if(levelOne == -1){
				netSnake.closeSocket();
				return false;
			}else if(levelOne == 1){ // Access Request Protocol
				if(!sendAccessRequest()){
					return false;
				}

				if(!keyExchange()){
					io.out(MORNING_IO_ERROR, "MLS Key exchange failed.\n");
					return false;
				}

				// Recv name request
				string msg = ctrRecv();
				if(msg == ""){
					netSnake.closeSocket();
					io.out(MORNING_IO_ERROR, "Failed to receive name request.\n");
					return false;
				}
				string name = io.inWithSpace(MORNING_IO_NONE, msg);
				if(!ctrSend(name, name.length())){
					netSnake.closeSocket();
					io.out(MORNING_IO_ERROR, "Failed to send server your name.\n");
					return false;
				}

				// Recv Reason for trust
				msg = ctrRecv();
				if(msg == ""){
                                        netSnake.closeSocket();
                                        io.out(MORNING_IO_ERROR, "Failed to receive request message prompt..\n");
                                        return false;
                                }
				string reason = io.inWithSpace(MORNING_IO_NONE, msg);
				if(!ctrSend(reason, reason.length())){
					netSnake.closeSocket();
					io.out(MORNING_IO_ERROR, "Failed to send reason to server.\n");
					return false;
				}
			
				// Receive succuess/failure message.
				msg = ctrRecv();
				if(msg == ""){
                                        netSnake.closeSocket();
                                        io.out(MORNING_IO_ERROR, "Failed to receive delivery confirmation\n");
                                        return false;
                                }

				netSnake.closeSocket();
				if(msg == "OK"){
					io.out(MORNING_IO_SUCCESS, "Your message has been received and is awaiting approval.\n");
					return true;
				}else{
					io.out(MORNING_IO_ERROR, "The server failed to store your approval request.\n");
					return false;
				}

			}else if(levelOne == 2){ // Try to authenticate with remote server
				if(!sendAuthRequest()){
					return false;
				}
				if(!keyExchange()){
                                        io.out(MORNING_IO_ERROR, "MLS Key exchange failed.\n");
                                        return false;
                                }

			}else{
				io.out(MORNING_IO_ERROR, "Invalid command attempted.\n");
			}
			netSnake.closeSocket();
			return true;
		}
};
