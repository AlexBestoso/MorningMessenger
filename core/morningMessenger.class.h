class MorningMessenger{
	private:
		const char *storageLocation = "./MMS_Storage";
		const char *configFile = "MMS_config.exml";
		const char *serverKeysDir = "serverKeys";
		const char *serverPubkey = "server.pub.key";
		const char *serverPrikey = "server.pri.key";
		const char *trustedKeysDir = "trustedKeys";
		const char *messagesDir = "messages";

		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;

		FileSnake fileSnake;
		EncryptionSnake encryptionSnake;

		string username = "";
		string password = "";
		string pin = "";

		string getConfigLoc(void){
			string dire = storageLocation;
			string fil = configFile;
			return dire + "/" + fil;
		}

		string getServerKeysDirLoc(void){
			string stem = storageLocation;
			string dire = serverKeysDir;
			return stem + "/" + dire;
		}

		string getServerPubKeyLoc(void){
			string keyFile = serverPubkey;
			string keyLoc = getServerKeysDirLoc();
			return keyLoc + "/" + keyFile;
		}

		string getServerPriKeyLoc(void){
			string keyFile = serverPrikey;
			string keyLoc = getServerKeysDirLoc();
			return keyLoc + "/" + keyFile;
		}

		string getTrustedKeysLoc(void){
			string stem = storageLocation;
			string dire = trustedKeysDir;
			return stem + "/" + dire;
		}

		string getMessagesLoc(void){
			string stem = storageLocation;
			string dire = messagesDir;
			return stem + "/" + dire;
		}

		void unlockConfig(void){
			size_t confSize = fileSnake.getFileSize(getConfigLoc());
                        char *buff = new char[confSize];
                        fileSnake.readFile(getConfigLoc(), buff, confSize);

                        string _key = algorithms.deriveConfigEncryptionKey(username, password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithms.deriveConfigEncryptionIv(username, password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];
			
			string _buff = "";
			for(int i=0; i<confSize; i++)
				_buff += buff[i];

                        string configEncrypted = encryptionSnake.aes256cbc(false, _buff, confSize, aesKey, aesIv);
                        delete[] buff;
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to decrypt config file.");
                        }

                        size_t encryptionResult = encryptionSnake.getResultLen();
			buff = new char[encryptionResult];
                        for(int i=0; i<encryptionResult; i++){
                                buff[i] = configEncrypted[i];
                        }
                        if(!fileSnake.writeFileTrunc(getConfigLoc(), buff, encryptionResult)){
                                throw MorningException("Failed to write decrypted config to file system.");
                        }
			delete[] buff;
		}

		void lockConfig(void){
			size_t confSize = fileSnake.getFileSize(getConfigLoc());
                        char *buff = new char[confSize];
                        fileSnake.readFile(getConfigLoc(), buff, confSize);

                        string _key = algorithms.deriveConfigEncryptionKey(username, password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithms.deriveConfigEncryptionIv(username, password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];

			string _buff = "";
			for(int i=0; i<confSize; i++)
				_buff += buff[i];
                        string configEncrypted = encryptionSnake.aes256cbc(true, _buff, confSize, aesKey, aesIv);
                        delete[] buff;
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to encrypt config file.");
                        }

                        size_t encryptionResult = encryptionSnake.getResultLen();
			buff = new char[encryptionResult];
			
			for(int i=0; i<encryptionResult; i++){
				buff[i] = configEncrypted[i];
			}
                        if(!fileSnake.writeFileTrunc(getConfigLoc(), buff, encryptionResult)){
                                throw MorningException("Failed to write encrypted config to file system.");
                        }
			delete[] buff;
		}

		void unlockPrivateKey(void){
			size_t priSize = fileSnake.getFileSize(getServerPriKeyLoc());
                        char *priBuff = new char[priSize];
                        fileSnake.readFile(getServerPriKeyLoc(), priBuff, priSize);

                        string _key = algorithms.deriveKeyEncryptionKey(username, password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithms.deriveKeyEncryptionIv(username, password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];

			string _buff = "";
                        for(int i=0; i<priSize; i++)
                                _buff += priBuff[i];


                        string priEncrypted = encryptionSnake.aes256cbc(false, _buff, priSize, aesKey, aesIv);
                        delete[] priBuff;
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to decrypt private key.");
                        }

                        size_t encryptionResult = encryptionSnake.getResultLen();
			priBuff = new char[encryptionResult];
			for(int i=0; i<encryptionResult; i++){
				priBuff[i] = priEncrypted[i];
			}
                        if(!fileSnake.writeFileTrunc(getServerPriKeyLoc(), priBuff, encryptionResult)){
                                throw MorningException("Failed to write decrypted private key to file system.");
                        }
			delete[] priBuff;
		}

		void lockPrivateKey(void){
			size_t priSize = fileSnake.getFileSize(getServerPriKeyLoc());
                        char *priBuff = new char[priSize];
                        fileSnake.readFile(getServerPriKeyLoc(), priBuff, priSize);

                        string _key = algorithms.deriveKeyEncryptionKey(username, password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithms.deriveKeyEncryptionIv(username, password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];

			string _buff = "";
                        for(int i=0; i<priSize; i++)
                                _buff += priBuff[i];

                        string priEncrypted = encryptionSnake.aes256cbc(true, _buff, priSize, aesKey, aesIv);
                        delete[] priBuff;
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to encrypt private key.");
                        }

                        size_t encryptionResult = encryptionSnake.getResultLen();
			priBuff = new char[encryptionResult];
                        for(int i=0; i<encryptionResult; i++){
                                priBuff[i] = priEncrypted[i];
                        }
                        if(!fileSnake.writeFileTrunc(getServerPriKeyLoc(), priBuff, encryptionResult)){
                                throw MorningException("Failed to write encrypted private key to file system.");
                        }
			delete[] priBuff;
		}
		void setupServerKeysDir(void){
			// Setup server keys dir
			io.out(MORNING_IO_GENERAL, "Setting up server keys directory\n");
			if(!fileSnake.fileExists(getServerKeysDirLoc())){
				if(!fileSnake.makeDir(getServerKeysDirLoc())){
					throw MorningException("Failed to make server keys directory");
				}else{
					io.outf(MORNING_IO_SUCCESS, "Created key directory '%s'\n", getServerKeysDirLoc().c_str());
				}
			}else if(fileSnake.getFileType(getServerKeysDirLoc()) != FILE_SNAKE_DIR){
				throw MorningException("'%s' already exists but is not a directory.", getServerKeysDirLoc().c_str());
			}else{
				io.outf(MORNING_IO_SUCCESS, "'%s' already exists!\n", getServerKeysDirLoc().c_str());
			}

			io.out(MORNING_IO_GENERAL, "Setting Server Keys Permissions to 600\n");
			if(!fileSnake.perm(getServerKeysDirLoc(), 00700)){
				throw MorningException("Failed to set server keys directory permissions.");
			}else{
				io.out(MORNING_IO_SUCCESS, "Successfully configured permissions!\n");
			}
		}
		void setupTrustedKeysDir(void){
			// Setup trusted keys dir.
			io.out(MORNING_IO_GENERAL, "Setting up trusted keys directory.\n");
			if(!fileSnake.fileExists(getTrustedKeysLoc())){
				if(!fileSnake.makeDir(getTrustedKeysLoc())){
					throw MorningException("Failed to make trusted keys directory.");
				}else{
					io.outf(MORNING_IO_SUCCESS, "Created trusted keys directory '%s'.\n", getTrustedKeysLoc().c_str());
				}
			}else if(fileSnake.getFileType(getTrustedKeysLoc()) != FILE_SNAKE_DIR){
				throw MorningException("'%s' already exists but is not a directory.", getTrustedKeysLoc().c_str());
			}else{
				io.outf(MORNING_IO_SUCCESS, "'%s' already exists!\n", getTrustedKeysLoc().c_str());
			}

			io.out(MORNING_IO_GENERAL, "Setting trusted keys directory to 600.\n");
			if(!fileSnake.perm(getTrustedKeysLoc(), 00700)){ // lol, for perm to be 600, you need to write it as '00600'
				throw MorningException("Failed to configure trusted keys permissions.");
			}else{
				io.out(MORNING_IO_SUCCESS, "Successfully configured permissions!\n");
			}
		}
		void setupMessagesDir(void){
			// Setup Messages directory.
			io.out(MORNING_IO_GENERAL, "Setting up messages directory.\n");
                        if(!fileSnake.fileExists(getMessagesLoc())){
                                if(!fileSnake.makeDir(getMessagesLoc())){
                                        throw MorningException("Failed to make messages directory.");
                                }else{
                                        io.outf(MORNING_IO_SUCCESS, "Created messages directory '%s'.\n", getMessagesLoc().c_str());
                                }
                        }else if(fileSnake.getFileType(getMessagesLoc()) != FILE_SNAKE_DIR){
                                throw MorningException("'%s' already exists but is not a directory.", getMessagesLoc().c_str());
                        }else{
                                io.outf(MORNING_IO_SUCCESS, "'%s' already exists!\n", getMessagesLoc().c_str());
                        }

                        io.out(MORNING_IO_GENERAL, "Setting messages directory to 600.\n");
                        if(!fileSnake.perm(getMessagesLoc(),  00700)){
                                throw MorningException("Failed to configure trusted keys permissions.");
                        }else{
                                io.out(MORNING_IO_SUCCESS, "Successfully configured permissions!\n");
                        }
		}
		
		void solicitUsername(void){
			username = io.inString(MORNING_IO_INPUT, "Enter username > ");
		}

		void solicitPassword(bool ensureCorrect){
			if(ensureCorrect){
				bool ensuring = true;
				while(ensuring){
					string test;
					string test2;
					test = io.inString(MORNING_IO_INPUT, "Enter a new password > ");
					test2 = io.inString(MORNING_IO_INPUT, "Re-enter password > ");
					if(test == test2){
						password = test;
						ensuring = false;
					}else{
						io.out(MORNING_IO_ERROR, "Passwords don't match. try again\n");
					}
				}
			}else{
				password = io.inString(MORNING_IO_INPUT, "Enter Password > ");
			}
		}
		void solicitPin(void){
			pin = io.inString(MORNING_IO_INPUT, "Enter Pin Number > ");
		}

		void generateServerKeys(string usr, string pass, string p){
			io.out(MORNING_IO_GENERAL, "Generating RSA Key Pair.\n");
			io.outf(MORNING_IO_GENERAL, "Saving public key to '%s'\n", getServerPubKeyLoc().c_str());
			io.outf(MORNING_IO_GENERAL, "Saving private key to '%s'\n", getServerPriKeyLoc().c_str());
			
			if(fileSnake.fileExists(getServerPubKeyLoc()))
				fileSnake.removeFile(getServerPubKeyLoc());
			if(fileSnake.fileExists(getServerPriKeyLoc()))
				fileSnake.removeFile(getServerPriKeyLoc());


			encryptionSnake.generateRsaKeyPairToFile(8192, false, getServerPubKeyLoc(), getServerPriKeyLoc(), algorithms.deriveExportKey(usr, pass, p));
			if(encryptionSnake.didFail()){
				encryptionSnake.printError();
				throw MorningException("Failed to generate server key pairs.");
			}
			io.out(MORNING_IO_SUCCESS, "Successfully generated server's key pair!\n");

			io.out(MORNING_IO_GENERAL, "Encrypting private server key.\n");
			lockPrivateKey();
			io.out(MORNING_IO_SUCCESS, "Encrypted private key successfuly!\n");
		}

		void generateDefaultConfig(void){
			io.out(MORNING_IO_GENERAL, "Generating default config file.\n");

			mornconf c;
			c.prikey = getServerPriKeyLoc();
			c.pubkey = getServerPubKeyLoc();
			c.trustedkeys = getTrustedKeysLoc();
			c.messages = getMessagesLoc();
			c.serverhost = "0.0.0.0";
			c.serverport = 21345;

			if(!config.newConfig(getConfigLoc(), c)){
				throw MorningException("Failed to create default config file '%s'", getConfigLoc().c_str());
			}else{
				io.outf(MORNING_IO_SUCCESS, "Successfully wrote the default configurationt to '%s'\n", getConfigLoc().c_str());
			}

			io.out(MORNING_IO_GENERAL, "Encrypting the config file.\n");
			lockConfig();
                        io.out(MORNING_IO_SUCCESS, "Encrypted config file successfuly!\n");
		}
		
	public:
	MorningMessenger(){
		try{
			if(!fileSnake.fileExists(getConfigLoc())){
				setupMessenger();
			}else{
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Login ========\n\n");
				solicitUsername();
                        	solicitPassword(false);
                        	solicitPin();
			}
			authenticateMessenger();
		}catch(exception &e){
			string what = e.what();
			what = "Failed to start up Morning Messenger.\nCaught in MorningMessenger::MorningMessenger() | " + what;
			throw MorningException(what);
		}
	}

	bool run(){
		bool running = true;
		while(running){
			// Post Setup entry point here.
		}	
		return true;
	}

	void authenticateMessenger(void){
		try{
			unlockConfig();
				config.loadConfig(getConfigLoc());	
			lockConfig();
			mornconf c = config.getConfig();
			io.out(MORNING_IO_SUCCESS, "Configuration loaded.\n");
			io.outf(MORNING_IO_GENERAL, "Loading public key '%s'\n", c.pubkey.c_str());
			encryptionSnake.fetchRsaKeyFromFile(false, false, false, c.pubkey, "");
			if(encryptionSnake.didFail()){
				encryptionSnake.cleanOutPublicKey();
				encryptionSnake.printError();
				throw MorningException("Failed to load public key.");
			}
			io.outf(MORNING_IO_GENERAL, "Unlocking and loading private key from '%s'\n", c.prikey.c_str());
			unlockPrivateKey();
				encryptionSnake.fetchRsaKeyFromFile(true, false, false, c.prikey, algorithms.deriveExportKey(username, password, pin));
                        	if(encryptionSnake.didFail()){
                        	        encryptionSnake.cleanOutPublicKey();
                        	        encryptionSnake.printError();
                        	        throw MorningException("Failed to load public key.");
                        	}
			lockPrivateKey();
			io.outf(MORNING_IO_SUCCESS, "Welcome Home, %s. You're authenticated.\n", username.c_str());

		}catch(exception &e){
			string what = e.what();
			what = "Authentication failure\nCaugth in MorningMessenger::authenticateMessenger() | " + what ;
			throw MorningException(what);
		}
	}

	void setupMessenger(void){
		try{
			setupServerKeysDir();
			setupTrustedKeysDir();
			setupMessagesDir();
			solicitUsername();
			solicitPassword(true);
			solicitPin();
			generateServerKeys(username, password, pin);
			generateDefaultConfig();
		}catch(exception &e){
			string what = e.what();
			what = "Faild to setup Morning Messenger.\nCaught in MorningMessenger::setupMessenger() | " + what;
			throw MorningException(what);
		}
	}
};
