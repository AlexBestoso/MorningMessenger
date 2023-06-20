struct morning_server_config{
	int id = -1;
	string publicKey = "";
	string privateKey = "";
	string keyPassword = "";
	string serverName = "";
};
typedef struct morning_server_config serverconfig_t;

class MorningServer{
	private:
		serverconfig_t serverConfig;
		string tableName = "morn_server";
		string colNames[5] = {
			"server_id",
			"server_pubkey",
			"server_prikey",
			"server_keypass",
			"server_name"
		};
		int sql_id = 0;
		int sql_pubkey = 1;
		int sql_prikey = 2;
		int sql_keypass = 3;
		int sql_name = 4;

		NetSnake netSnake;
		FileSnake fileSnake;
		EncryptionSnake encryptionSnake;
		EncryptionSnake remoteCerts;
		XmlSnake xmlSnake;
		SqlSnake sqlSnake;
	
		MorningAlgorithms algorithm;
		MorningConfig config;
		MorningKeyManager keyManager;
		MorningMessage morningMessage;

		string cmd_newUser = "a235210cd5476dfa0a045d60244e5cc6aebbc21e406fba344ce5d154b6337f5b";
		string cmd_existingUser = "f0773273589a87e67e71f402c08c93b464ff307846de3e7567dc1e423d65baf9";
		string cmd_streamChat = "6c0df3e03ccc8a63b41937acc7169cabad337da6bc7bb36a19245548d6eef2a3";
		string cmd_chat = "7b5a3431691d02a5f335235bae96e65301311115d3688e2306c0a664ab29bfbe";
		
		string clientPublicKey = "";
		string clientName = "";
		string clientMsg = "";

		size_t rsaBufSize = 1024;
		size_t ctrBufSize = 5000;

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
			size_t keySize = 1466;
			string key = "";
			char buffer[1466];// = new char[6];

			// recveive key size
			if(!netSnake.serverRecv(buffer, keySize, 0)){
				netSnake.closeConnection();
				return false;
			}
			
			clientPublicKey = "";
			for(int i=0; i<netSnake.server_recvSize; i++)
				clientPublicKey += buffer[i];

			// Ensure full key is received.
                        int remaining = keySize - netSnake.server_recvSize;
                        if(remaining > 0 && netSnake.server_recvSize > 0){
				memset(buffer, 0x00, keySize);
                                if(!netSnake.serverRecv(buffer, remaining, 0)){
                                        netSnake.closeSocket();
                                        return false;
                                }

                                for(int i=0; i<netSnake.server_recvSize; i++)
                                        clientPublicKey += buffer[i];
                                remaining = remaining - netSnake.server_recvSize;
                        }

			encryptionSnake.cleanOutPublicKey();
			encryptionSnake.fetchRsaKeyFromString(false, false, clientPublicKey, clientPublicKey.length(), "");
			if(encryptionSnake.didFail()){
				encryptionSnake.printError();
				netSnake.closeConnection();
				clientPublicKey = "";
				return false;
			}
			return true;
		}

		bool sendPublicKey(void){
			mornconf cfg = config.getConfig();
                        size_t size = fileSnake.getFileSize(cfg.pubkey);

                        char *buffer = new char[size];
                        if(!fileSnake.readFile(cfg.pubkey, buffer, size)){
                                netSnake.closeConnection();
                                delete[] buffer;
                                return false;
                        }
			string key = "";
                        for(int i=0; i<size; i++)
                                key += buffer[i];
                        delete[] buffer;

                        if(!netSnake.serverSend(key.c_str(), size)){
                                netSnake.closeConnection();
                                return false;
                        }
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

		bool ctrSend(string msg, size_t msgLen){
			string encryptedMsg = encryptionSnake.aes256ctr_execute(true, msg, msgLen);
			if(encryptionSnake.didFail()){
				return false;
			}

			if(!netSnake.serverSend(encryptedMsg.c_str(), encryptionSnake.getResultLen())){
				return false;
			}
			return true;	
		}
		string ctrRecv(void){
			string ret = "";
			char *recvBuffer = new char[ctrBufSize];
			if(!netSnake.serverRecv(recvBuffer, ctrBufSize, 0)){
				delete[] recvBuffer;
				return "";
			}
			for(int i=0; i<netSnake.server_recvSize; i++){
				ret += recvBuffer[i];
			}
			delete[] recvBuffer;

			ret = encryptionSnake.aes256ctr_execute(false, ret, netSnake.server_recvSize);
			if(encryptionSnake.didFail()){
				return "";
			}

			return ret;
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
			if(msgLen > 1024)
				msgLen = 1024;
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

		bool storeMessageRequest(void){
			try{
				bool ret = keyManager.createUntrusted(clientPublicKey, clientName, clientMsg);
				return ret;
			}catch(exception &e){
				return false;
			}
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

						string msg = "What is your name? > ";
						if(!ctrSend(msg, msg.length())){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}
						
						clientName = ctrRecv();
						if(clientName == ""){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						msg = "Why should we trust you? > ";
						if(!ctrSend(msg, msg.length())){
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
                                                }

						clientMsg = ctrRecv();
						if(clientMsg == ""){
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
                                                }


						if(!storeMessageRequest()){
							msg = "NO";
							ctrSend(msg, msg.length());
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}
							
						msg = "OK";
						ctrSend(msg, msg.length());
						netSnake.closeConnection();
						exit(EXIT_SUCCESS);
					}else if(cmdOne == 2){ // Authenticate existing user
						if(!keyExchange()){
                                                        exit(EXIT_FAILURE);
                                                }
						
						if(!keyManager.isKeyTrusted(clientPublicKey)){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						mornmsg clientMessage;
						clientMessage.clientHost = netSnake.getClientIp();
						clientMessage.messageDate = morningMessage.getCurrentDateTime();
						clientMessage.messageBody = ctrRecv();
						if(clientMessage.messageBody == ""){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}
							
						try{
						morningMessage.setConfig(config);
						if(!morningMessage.storeClientMessage(clientMessage, clientPublicKey)){
							string resp = "[E] Failed to store your message.";
							ctrSend(resp, resp.length());
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}else{
							string resp = "[+] Succefully received your message.";
							ctrSend(resp, resp.length());
							netSnake.closeConnection();
							exit(EXIT_SUCCESS);
						}}catch(exception &e){
							string resp = e.what();
							resp = "[E]" + resp;	
                                                        ctrSend(resp, resp.length());
                                                        netSnake.closeConnection();
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
	
	sqltable_t generateTable(void){
		sqltable_t ret;
		ret.name = tableName;
		ret.colCount = 5;
		ret.cols = new sqlcolumn_t[5];
		ret.cols[sql_id] = sqlSnake.generatePrimaryColumn(colNames[sql_id], "INT", "NOT NULL AUTO_INCREMENT");
		ret.cols[sql_pubkey] = sqlSnake.generateColumn(colNames[sql_pubkey], "TEXT", "NOT NULL");
                ret.cols[sql_prikey] = sqlSnake.generateColumn(colNames[sql_prikey], "TEXT", "NOT NULL");
                ret.cols[sql_keypass] = sqlSnake.generateColumn(colNames[sql_keypass], "VARCHAR(100)", "NOT NULL");
                ret.cols[sql_name] = sqlSnake.generateColumn(colNames[sql_name], "VARCHAR(25)", "NOT NULL");
		return ret;
	}

	sqlinsert_t generateInsert(string pubkey, string prikey, string keypass, string name){
		sqlinsert_t ret;
		ret.table = tableName;
		ret.count = 4;
		ret.cols = new string[4]{
			colNames[sql_pubkey],
			colNames[sql_prikey],
			colNames[sql_keypass],
			colNames[sql_name]
		};
		ret.values = new string[4]{
			pubkey,
			prikey,
			keypass,
			name
		};
		return ret;
	}

	void setupServerTable(SqlSnake sqlSnake){
		if(fileSnake.fileExists(config.getServerPubKeyLoc()))
			fileSnake.removeFile(config.getServerPubKeyLoc());
		if(fileSnake.fileExists(config.getServerPriKeyLoc()))
			fileSnake.removeFile(config.getServerPriKeyLoc());

		string keypass = encryptionSnake.sha256(encryptionSnake.randomPrivate(100), 100, false);
		if(encryptionSnake.didFail()){
			encryptionSnake.printError();
			throw MorningException("Failed to generate private key password.");
		}
	
		encryptionSnake.generateRsaKeyPairToFile(8192, false, config.getServerPubKeyLoc(), config.getServerPriKeyLoc(), keypass);
		if(encryptionSnake.didFail()){
			encryptionSnake.printError();
			throw MorningException("Failed to generate server key pairs.");
		}

		sqltable_t t = generateTable();
		if(!sqlSnake.createTable(t)){
			fileSnake.removeFile(config.getServerPubKeyLoc());
			fileSnake.removeFile(config.getServerPriKeyLoc());
			throw MorningException(sqlSnake.getError());
		}

		string pkey = "";
		size_t pkey_size = fileSnake.getFileSize(config.getServerPubKeyLoc());
		char *pkey_buf = new char[pkey_size];
		if(!fileSnake.readFile(config.getServerPubKeyLoc(), pkey_buf, pkey_size)){
			fileSnake.removeFile(config.getServerPubKeyLoc());
			fileSnake.removeFile(config.getServerPriKeyLoc());
			throw MorningException("Failed to load in generated public key.");
		}else{
			for(int i=0; i<pkey_size; i++)
				pkey += pkey_buf[i];
			fileSnake.removeFile(config.getServerPubKeyLoc());
		}
		delete[] pkey_buf;

		string prkey = "";
		size_t prkey_size = fileSnake.getFileSize(config.getServerPriKeyLoc());
		char *prkey_buf = new char[prkey_size];
		if(!fileSnake.readFile(config.getServerPriKeyLoc(), prkey_buf, prkey_size)){
			fileSnake.removeFile(config.getServerPriKeyLoc());
			throw MorningException("Failed to load in generated private key.");
		}else{
			for(int i=0; i<prkey_size; i++)
				prkey += prkey_buf[i];
			fileSnake.removeFile(config.getServerPriKeyLoc());
		}
		delete[] prkey_buf;

		string sname = "anonymous";
	
		sqlinsert_t insert = generateInsert(pkey, prkey, keypass, sname); 
		if(!sqlSnake.secureInsert(insert)){
			throw MorningException(sqlSnake.getError());
		}
	}
};
