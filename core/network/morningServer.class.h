struct morning_server_config{
	int id = -1;
	string publicKey = "";
	string privateKey = "";
	string keyPassword = "";
	string serverName = "";
	string serverHost = "";
	int serverPort = -1;
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
		
		friendkey_t friendKey;

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
			
			friendKey.pubkey = "";
			for(int i=0; i<netSnake.server_recvSize; i++)
				friendKey.pubkey += buffer[i];

			// Ensure full key is received.
                        int remaining = keySize - netSnake.server_recvSize;
                        if(remaining > 0 && netSnake.server_recvSize > 0){
				memset(buffer, 0x00, keySize);
                                if(!netSnake.serverRecv(buffer, remaining, 0)){
                                        netSnake.closeSocket();
                                        return false;
                                }

                                for(int i=0; i<netSnake.server_recvSize; i++)
                                        friendKey.pubkey += buffer[i];
                                remaining = remaining - netSnake.server_recvSize;
                        }

			encryptionSnake.cleanOutPublicKey();
			encryptionSnake.fetchRsaKeyFromString(false, false, friendKey.pubkey, friendKey.pubkey.length(), "");
			if(encryptionSnake.didFail()){
				encryptionSnake.printError();
				netSnake.closeConnection();
				friendKey.pubkey = "";
				return false;
			}
			return true;
		}

		bool sendPublicKey(void){
			loadConfigs();
			string key = serverConfig.publicKey;

                        if(!netSnake.serverSend(key.c_str(), key.length())){
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
				bool ret = keyManager.createUntrusted(friendKey);
				return ret;
			}catch(exception &e){
				return false;
			}
		}

		void playRequestReceived(){
			SoundSnake ss;
			size_t fileSize = fileSnake.getFileSize("/var/morningService/sounds/messageRequest.wav");
			char *fileBuff = new char[fileSize];
			fileSnake.readFile("/var/morningService/sounds/messageRequest.wav", fileBuff, fileSize);
			
			if(!ss.initSimpleSpeaker("MorningServer"))
				return;
			ss.playSimpleAudio(fileBuff, fileSize);
			ss.freeDevice();
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
			config.loadConfig();
			loadConfigs();
			if(!netSnake.createInetServer(serverConfig.serverPort)){
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

						string msg = "What is your name?";
						if(!ctrSend(msg, msg.length())){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}
						
						friendKey.alias = ctrRecv();
						if(friendKey.alias == ""){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						msg = "Why should we trust you?";
						if(!ctrSend(msg, msg.length())){
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
                                                }

						friendKey.justification = ctrRecv();
						if(friendKey.justification == ""){
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
                                                }

						
						friendKey.cbhost = ctrRecv();
						if(friendKey.cbhost == ""){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						friendKey.cbport = atoi(ctrRecv().c_str());
						if(!(friendKey.cbport > 0 && friendKey.cbport <= 65535)){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}
						friendKey.serverAlias = ctrRecv();
						if(friendKey.serverAlias == ""){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						friendKey.cbip = netSnake.getClientIp();
						friendKey.trusted = false;
						friendKey.date = "";
						if(storeMessageRequest() == false){
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
						
						if(!keyManager.isKeyTrusted(friendKey.pubkey)){
							netSnake.closeConnection();
							exit(EXIT_FAILURE);
						}

						mornmsg clientMessage;

						string xmlResponse = ctrRecv();
						if(xmlResponse == ""){
							string resp = "[E] failed to receive your response.";
                                                        ctrSend(resp, resp.length());
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
						}
							
						if(!xmlSnake.openStringReader(xmlResponse)){
							string resp = "[E] failed to process your message.";
                                                        ctrSend(resp, resp.length());
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
						}
						string previous = "";
						while(xmlSnake.readLineReader()){
							if(xmlSnake.readResult.name == "#text" && previous == "selfDestruct"){
                                        			clientMessage.selfDestruct = (xmlSnake.readResult.value == "true") ? true : false;
                                			}
							if(xmlSnake.readResult.name == "#text" && previous == "subject"){
								clientMessage.subject = xmlSnake.readResult.value;
							}
							if(xmlSnake.readResult.name == "#text" && previous == "message"){
								clientMessage.message = xmlSnake.readResult.value;
							}
							if(xmlSnake.readResult.name == "#text" && previous == "messageSize"){
								clientMessage.messageSize = atoi(xmlSnake.readResult.value.c_str());
							}
                                			previous = xmlSnake.readResult.name;

						}
						xmlSnake.closeReader();
						
						clientMessage.keyId = keyManager.getIdByKey(friendKey.pubkey);
						if(clientMessage.keyId <= 0){
							string resp = "[E] Your key's not in the database.";
                                                        ctrSend(resp, resp.length());
                                                        netSnake.closeConnection();
                                                        exit(EXIT_FAILURE);
						}
						try{
							if(!morningMessage.storeClientMessage(clientMessage)){
								string resp = "[E] Failed to store your message : \n"+morningMessage.err;
								ctrSend(resp, resp.length());
								netSnake.closeConnection();
								exit(EXIT_FAILURE);
							}else{
								string resp = "[+] Succefully received your message.";
								ctrSend(resp, resp.length());
								netSnake.closeConnection();
								exit(EXIT_SUCCESS);
							}
						}catch(exception &e){
							string resp = e.what();
							resp = "[E] UNKNOWN ERROR: " + resp;	
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

	serverconfig_t generateNewKeyPair(void){
		SqlSnake sqlSnake = config.getSql();
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

		serverconfig_t newCfg = serverConfig;
		newCfg.publicKey = pkey;
		newCfg.privateKey = prkey;
		newCfg.keyPassword = keypass;
		return newCfg;
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

		loadConfigs();
	}

	void loadConfigs(){
		SqlSnake sqlSnake = config.getSql();
		sqlselect_t select;
		select.table = tableName;
		select.cols = colNames;
		select.colCount = 5;
		if(!sqlSnake.secureSelect(select)){
			throw MorningException(sqlSnake.getError());
		}

		sqlresults_t res = sqlSnake.getResults();
		if(res.resultCount <= 0){
			throw MorningException("Server configuration not set up.");
		}

		if(res.fieldCount != 5)
			throw MorningException("Server configuration table has invalid field count.");

		serverConfig.id = atoi(res.results[0].values[sql_id].c_str());
		serverConfig.publicKey = sqlSnake.desanitize(res.results[0].values[sql_pubkey]);
		serverConfig.privateKey = sqlSnake.desanitize(res.results[0].values[sql_prikey]);
		serverConfig.keyPassword = res.results[0].values[sql_keypass];
		serverConfig.serverName = res.results[0].values[sql_name];


		config.loadConfig();
		morningconfig_t cfg = config.getConfig();
		serverConfig.serverHost = cfg.serviceHost;
		serverConfig.serverPort = cfg.servicePort;
		
	}

	void updateConfig(serverconfig_t newCfg){
		config.loadConfig();
		SqlSnake sqlSnake = config.getSql();

		sqlupdate_t update;
		update.table = tableName;
		update.valueCount = 4;
		update.cols = new string[4]{
			colNames[sql_pubkey],
                	colNames[sql_prikey],
                	colNames[sql_keypass],
                	colNames[sql_name],
		};
		update.values = new string[4]{
			newCfg.publicKey,
        		newCfg.privateKey,
        		newCfg.keyPassword,
        		newCfg.serverName
		};

		update.wheres = sqlSnake.addToWhere(update.wheres, 
				sqlSnake.generateWhere(colNames[sql_id], "!=", "-1", true), 
				""
		);
		if(!sqlSnake.secureUpdate(update)){
			throw MorningException("Failed to updated server config : %s", sqlSnake.getError());
		}

		morningconfig_t cfg = config.getConfig();
		cfg.serviceHost = newCfg.serverHost;
		cfg.servicePort = newCfg.serverPort;
		config.generateConfig(cfg);
	}

	serverconfig_t getServerConfig(){
		return serverConfig;
	}
};
