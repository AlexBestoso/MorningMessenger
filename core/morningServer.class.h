class MorningServer{
	private:
		NetSnake netSnake;
		FileSnake fileSnake;
		EncryptionSnake encryptionSnake;
		EncryptionSnake remoteCerts;
	
		MorningAlgorithms algorithm;
		MorningConfig config;

		string cmd_newUser = "a235210cd5476dfa0a045d60244e5cc6aebbc21e406fba344ce5d154b6337f5b";
		string cmd_existingUser = "f0773273589a87e67e71f402c08c93b464ff307846de3e7567dc1e423d65baf9";
		string cmd_streamChat = "6c0df3e03ccc8a63b41937acc7169cabad337da6bc7bb36a19245548d6eef2a3";
		string cmd_chat = "7b5a3431691d02a5f335235bae96e65301311115d3688e2306c0a664ab29bfbe";
		
		string clientPublicKey = "";

		size_t rsaBufSize = 2000;

		int commandLevelOne(void){
			size_t cmdSize = 64;
			char *cmdBuff = new char[cmdSize];
			if(!netSnake.serverRecv(cmdBuff, cmdSize, 0)){
				delete[] cmdBuff;
				return -1;
			}
			
			string conv = "";
			for(int i=0; i<netSnake.server_recvSize; i++)
				conv += cmdBuff[i];
			delete[] cmdBuff;

			if(conv == cmd_newUser){
				sendOkay();
				return 1;
			}else if(conv == cmd_existingUser){
				sendOkay();
				return 2;
			}
			sendNo();
			return -1;
		}

		void sendOkay(void){
			netSnake.serverSend("OK", 2);
		}

		void sendNo(void){
			netSnake.serverSend("NO", 2);
		}

		bool recvPublicKey(void){
			size_t keySize = 0;
			string key = "";
			char *buffer = new char[6];
			// recveive key size
			if(!netSnake.serverRecv(buffer, 6, 0)){
				netSnake.closeConnection();
				delete[] buffer;
				return false;
			}

			keySize = atoi(buffer);
			delete[] buffer;
			if(keySize <= 0){
				netSnake.closeConnection();
				return false;
			}
			buffer = new char[keySize];
			if(!netSnake.serverRecv(buffer, keySize, 0)){
				netSnake.closeConnection();
				return false;
			}
			
			clientPublicKey = "";
			for(int i=0; i<netSnake.server_recvSize; i++)
				clientPublicKey += buffer[i];
			delete[] buffer;

			encryptionSnake.cleanOutPublicKey();
			encryptionSnake.fetchRsaKeyFromString(false, false, clientPublicKey.c_str(), netSnake.server_recvSize, "");
			if(encryptionSnake.didFail()){
				netSnake.closeConnection();
				clientPublicKey = "";
				return false;
			}
			return true;
		}

		bool sendPublicKey(void){
			mornconf cfg = config.getConfig();
                        size_t size = fileSnake.getFileSize(cfg.pubkey);
                        string keySize = to_string(size);
                        if(!netSnake.serverSend((char *)keySize.c_str(), keySize.length())){
                                netSnake.closeConnection();
                                return false;
                        }

                        char *buffer = new char[size];
                        if(!fileSnake.readFile(cfg.pubkey, buffer, size)){
                                netSnake.closeConnection();
                                delete[] buffer;
                                return false;
                        }

                        if(!netSnake.serverSend(buffer, size)){
                                netSnake.closeConnection();
                                delete[] buffer;
                                return false;
                        }
                        delete[] buffer;
                        return true;
		}


		bool generateAndSendCtrKey(void){
			encryptionSnake.aes256ctr_stop(true);
                        encryptionSnake.aes256ctr_stop(false);

			string keyPacket = "";
			// IV is 16 bytes
			string _iv = algorithm.deriveRandomIv();
			unsigned char iv[16];
			for(int i=0; i<16; i++){
				iv[i] = _iv[i];
				keyPacket += _iv[i];
			}
			// Key is 32 bytes
			string _key = algorithm.deriveRandomKey();
			unsigned char key[32];
			for(int i=0; i<32; i++){
				key[i] = _key[i];
				keyPacket += _key[i];
			}

			if(!rsaSend(keyPacket, 32+16)){
				return false;
			}

			if(!encryptionSnake.aes256ctr_start(true, key, iv)){
				return false;
			}

			if(!encryptionSnake.aes256ctr_start(false, key, iv)){
				return false;
			}
			return true;
		}

		bool keyExchange(void){
			if(!recvPublicKey()){
				netSnake.closeConnection();
				return false;
			}
			if(!sendPublicKey()){
				netSnake.closeConnection();
				return false;
			}

			if(!generateAndSendCtrKey()){
				netSnake.closeConnection();
				return false;
			}
			return true;
		}

		bool rsaSend(string msg, size_t msgLen){
			string encryptedMessage = encryptionSnake.rsa(true, msg, msgLen);
			if(encryptionSnake.didFail()){
				return false;
			}
			size_t encryptedMessageLen = encryptionSnake.getResultLen();

			if(!netSnake.serverSend(encryptedMessage.c_str(), encryptedMessageLen)){
				return false;
			}

			return true;
		}

		string rsaRecv(void){
			string ret = "";
			char *buf = new char[rsaBufSize];
			if(!netSnake.serverRecv(buf, rsaBufSize, 0)){
				return "";
			}
			size_t recvSize = netSnake.server_recvSize;
			for(int i=0; i<recvSize; i++)
				ret += buf[i];

			ret = encryptionSnake.rsa(false, ret, recvSize);
			if(encryptionSnake.didFail()){
				return "";
			}
			return ret;
		}
	public:
	pid_t pid = -1;
	void setConfig(MorningConfig config){
		this->config = config;
	}
	void setEncryptionSnake(EncryptionSnake encryptionSnake){
		this->encryptionSnake = encryptionSnake;
	}
	void clearLockFile(void){
		string lockFile = config.getServerLockFile();
		fileSnake.removeFile(lockFile);
	}

	bool lockHeld(void){
		string lockFile = config.getServerLockFile();
		return fileSnake.fileExists(lockFile);
	}

	void killProcess(){
		kill(pid, SIGKILL);
                clearLockFile();
                pid = -1;
	}

	bool obtainLockFile(void){
		string lockFile = config.getServerLockFile();
		if(fileSnake.fileExists(lockFile))
			return false;
		if(!fileSnake.writeFileTrunc(lockFile, (char *)"1", 1))
			throw MorningException("Failed to obtain lock file.");
		return true;
	}

	void launchServer(){
		try{
			mornconf cfg = config.getConfig();
			if(!netSnake.createInetServer(cfg.serverport)){
				throw MorningException(netSnake.errorMessage());
			}
		
			bool running = true;
			while(running){
				if(!netSnake.listenAndConnect()){
					throw MorningException(netSnake.errorMessage());
				}
				if(fork() == 0){
					int cmdOne = commandLevelOne();
					if(cmdOne == -1){
						netSnake.closeConnection();
						exit(EXIT_FAILURE);
					}else if(cmdOne == 1){ // Setup new user
						if(!keyExchange()){
							exit(EXIT_FAILURE);
						}

					}else if(cmdOne == 2){ // Authenticate existing user
						if(!keyExchange()){
                                                        exit(EXIT_FAILURE);
                                                }

					}
					netSnake.closeConnection();
					exit(EXIT_SUCCESS);
				}
				netSnake.closeConnection();
			}
		}catch(exception &e){
			string what = e.what();
			what = "MorningServer::launchServer() | Critical Failure.\n" + what;
			throw MorningException(what);
		}
	}	
};
