struct morning_config_xml{
	string messages;
	string prikey;
	string pubkey;
	string serverhost;
	int serverport;
	string trustedkeys;
	
};
typedef struct morning_config_xml mornconf;

struct morning_config{
	string sqlHost = "";
	unsigned int sqlPort= 3306;
	string sqlUser = "";
	string sqlPass = "";
	string sqlDatabase = "mornmessage";
	string serviceHost = "";
	int servicePort = 21345;
};
typedef struct morning_config morningconfig_t;

class MorningConfig{
	private:
		/* Variables  */
		XmlSnake xml;
		EncryptionSnake encryptionSnake;
		FileSnake fileSnake;
		SqlSnake sqlSnake;

		MorningIO io;
		MorningAlgorithms algorithms;
		MorningUser user;
		
		mornconf config;
		morningconfig_t sqlconf;

		const char *storageLocation = "./MMS_Storage";
		const char *serverLockFile = "server.lock";
                const char *configFile = "MMS_config.exml";
                const char *serverKeysDir = "serverKeys";
                const char *serverPubkey = "server.pub.key";
                const char *serverPrikey = "server.pri.key";
                const char *trustedKeysDir = "trustedKeys";
		const char *untrustedKeysDir = "sketchyKeys";
                const char *messagesDir = "messages";

		/* Functions */

                string getServerKeysDirLoc(void){
                        string stem = storageLocation;
                        string dire = serverKeysDir;
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

		__attribute__((deprecated("Obsolete function will be removed in newer versions")))void setupServerKeysDir(void){
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

		void setupUntrustedKeysDir(void){
                        // Setup trusted keys dir.
                        io.out(MORNING_IO_GENERAL, "Setting up untrusted keys directory.\n");
                        if(!fileSnake.fileExists(getUntrustedKeysLoc())){
                                if(!fileSnake.makeDir(getUntrustedKeysLoc())){
                                        throw MorningException("Failed to make unrusted keys directory.");
                                }else{
                                        io.outf(MORNING_IO_SUCCESS, "Created untrusted keys directory '%s'.\n", getUntrustedKeysLoc().c_str());
                                }
                        }else if(fileSnake.getFileType(getUntrustedKeysLoc()) != FILE_SNAKE_DIR){
                                throw MorningException("'%s' already exists but is not a directory.", getUntrustedKeysLoc().c_str());
                        }else{
                                io.outf(MORNING_IO_SUCCESS, "'%s' already exists!\n", getUntrustedKeysLoc().c_str());
                        }

                        io.out(MORNING_IO_GENERAL, "Setting untrusted keys directory to 600.\n");
                        if(!fileSnake.perm(getUntrustedKeysLoc(), 00700)){ // lol, for perm to be 600, you need to write it as '00600'
                                throw MorningException("Failed to configure untrusted keys permissions.");
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

		__attribute__((deprecated("Obsolete function being removed in new versions.")))void generateServerKeys(string usr, string pass, string p){
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

		__attribute__((deprecated("obselete function is being removed in future versions.")))void generateDefaultConfig(void){
                        io.out(MORNING_IO_GENERAL, "Generating default config file.\n");

                        mornconf c;
                        c.prikey = getServerPriKeyLoc();
                        c.pubkey = getServerPubKeyLoc();
                        c.trustedkeys = getTrustedKeysLoc();
                        c.messages = getMessagesLoc();
                        c.serverhost = "0.0.0.0";
                        c.serverport = 21345;

                        if(!newConfig(getConfigLoc(), c)){
                                throw MorningException("Failed to create default config file '%s'", getConfigLoc().c_str());
                        }else{
                                io.outf(MORNING_IO_SUCCESS, "Successfully wrote the default configurationt to '%s'\n", getConfigLoc().c_str());
                        }
                }
		void setupDatabase(){
			if(!sqlSnake.init(sqlconf.sqlHost, sqlconf.sqlPort, sqlconf.sqlUser, sqlconf.sqlPass, sqlconf.sqlDatabase)){
				throw MorningException(sqlSnake.getError());
			}
			if(!sqlSnake.createDatabase(sqlconf.sqlDatabase)){
				sqlSnake.close();
				throw MorningException(sqlSnake.getError());

			}
			if(!sqlSnake.useDatabase(sqlconf.sqlDatabase)){
				sqlSnake.close();
				throw MorningException(sqlSnake.getError());
			}

			MorningUser usr;
			if(!sqlSnake.createTable(usr.buildTable())){
				sqlSnake.close();
				throw MorningException(sqlSnake.getError());
			}
			sqlSnake.close();
		}

	public:
		string username = "";
		string password = "";
		string pin = "";

		__attribute__((deprecated("Obselete function being removed in future verions.")))mornconf getConfig(void){
			return config;
		}

		morningconfig_t getSqlConfig(){
			return sqlconf;
		}
		

		SqlSnake getSql(void){
			return sqlSnake;
		}

		string getConfigLoc(void){
                        string dire = storageLocation;
                        string fil = configFile;
                        return dire + "/" + fil;
                }

		string getServerLockFile(void){
			string dire = storageLocation;
			string fil = serverLockFile;
			return dire + "/" + serverLockFile;
		}

		string getTrustedKeysLoc(void){
                        string stem = storageLocation;
                        string dire = trustedKeysDir;
                        return stem + "/" + dire;
                }

                string getUntrustedKeysLoc(void){
                        string stem = storageLocation;
                        string dire = untrustedKeysDir;
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

		void setSessionCreds(string u, string p, string pin){
			username = u;
			password = p;
			this->pin = pin;
		}


		void setupMessenger(morningconfig_t cfg){
			try{
				generateConfig(cfg);
				setupDatabase();
				setupServerKeysDir();

			}catch(exception &e){
				string what = e.what();
                                what = "Faild to setup Morning Messenger.\nCaught in MorningMessenger::setupMessenger() | " + what;
                                throw MorningException(what);
			}	
		}

		__attribute__((deprecated("Obselete function being removed in future versions.")))void setupMessenger(string username, string password, string pin){
                	try{
				this->username = username;
			       	this->password = password;
				this->pin = pin;
                       	 	setupServerKeysDir();
                	       	setupTrustedKeysDir();
				setupUntrustedKeysDir();
                	        setupMessagesDir();
                	        generateServerKeys(username, password, pin);
                	        generateDefaultConfig();
                	}catch(exception &e){
                	        string what = e.what();
                	        what = "Faild to setup Morning Messenger.\nCaught in MorningMessenger::setupMessenger() | " + what;
                	        throw MorningException(what);
                	}
        	}

		bool generateConfig(morningconfig_t config){
			string fileName = getConfigLoc();
			sqlconf = config;
			if(!xml.openFileWriter(fileName))
				throw MorningException("Failed to open '%s' for writing.", fileName.c_str());
			if(!xml.startWritingFile())
                                throw MorningException("Failed to start writing config file.");
                        if(!xml.startWritingElement("root"))
                                throw MorningException("Failed to create <root> element.");


			if(!xml.writeElement("sqlhost", sqlconf.sqlHost))
				throw MorningException("Failed to write sql host to config file.");
			if(!xml.writeElement("sqlport", to_string(sqlconf.sqlPort)))
				throw MorningException("Failed to write sql port to config file.");
			if(!xml.writeElement("sqluser", sqlconf.sqlUser))
				throw MorningException("Failed to write sql user to config file.");
			if(!xml.writeElement("sqlpass", sqlconf.sqlPass))
				throw MorningException("Failed to write sql password to config file.");
			if(!xml.writeElement("sqldatabase", sqlconf.sqlDatabase))
				throw MorningException("Failed to write sql database to config file.");
			if(!xml.writeElement("servicehost", sqlconf.serviceHost))
                                throw MorningException("Failed to write service host to config file.");
                        if(!xml.writeElement("serviceport", to_string(sqlconf.servicePort)))
                                throw MorningException("Failed to write service port to config file.");

			if(!xml.stopWritingElement())
                                throw MorningException("Failed to stop writing trusted keys storage location to config file.");
                        if(!xml.stopWritingFile())
                                throw MorningException("Failed to stop writing file.");
                        xml.closeFileWriter();

			return true;
		}

		__attribute__((deprecated("obselete config function is being removed in future versions."))) bool newConfig(string fileName, mornconf config){
			this->config = config;

			if(!xml.openFileWriter(fileName))
				throw MorningException("Failed to open '%s' for writing.", fileName.c_str());
			if(!xml.startWritingFile())
				throw MorningException("Failed to start writing config file.");
			if(!xml.startWritingElement("root"))
				throw MorningException("Failed to create <root> element.");
			/* Begin creating config body*/
			
			if(!xml.writeElement("messages", config.messages))
				throw MorningException("Failed to write message storage to config file.");

			if(!xml.writeElement("prikey", config.prikey))
				throw MorningException("Failed to write private key location to config file.");

			if(!xml.writeElement("pubkey", config.pubkey))
				throw MorningException("Failed to write public key location to config file.");

			if(!xml.writeElement("serverhost", config.serverhost))
				throw MorningException("Failed to write server host to config file.");
			
			string port_str = to_string(config.serverport);
			if(!xml.writeElement("serverport", port_str))
				throw MorningException("Failed to write server port to config file.");

			if(!xml.writeElement("trustedkeys", config.trustedkeys))
				throw MorningException("Failed to write trusted keys storage location to config file.");
			
			/* Crafting done, level 99 skill*/
			if(!xml.stopWritingElement())
				throw MorningException("Failed to stop writing trusted keys storage location to config file.");
			if(!xml.stopWritingFile())
				throw MorningException("Failed to stop writing file.");
			xml.closeFileWriter();
			
			lockConfig();
			return true;
		}

		bool loadConfig(void){
			string fileName = getConfigLoc();
			//unlockConfig();
			if(!xml.openFileReader(fileName)){
			//	lockConfig();
				throw MorningException("Failed to open config file '%s'", fileName.c_str());
			}

			string previous = "";
			while(xml.readLineReader()){
				if(xml.readResult.name == "#text" && previous == "sqlport"){
					 sqlconf.sqlPort = (unsigned int)atoi(xml.readResult.value.c_str());
				}
				if(xml.readResult.name == "#text" && previous == "sqluser"){
					sqlconf.sqlUser = xml.readResult.value;
				}
				if(xml.readResult.name == "#text" && previous == "sqlpass"){
					sqlconf.sqlPass = xml.readResult.value;
				}
				if(xml.readResult.name == "#text" && previous == "sqlhost"){
					sqlconf.sqlHost = xml.readResult.value;
				}
				if(xml.readResult.name == "#text" && previous == "sqldatabase"){
					sqlconf.sqlDatabase = xml.readResult.value.c_str();
				}
				if(xml.readResult.name == "#text" && previous == "servicehost"){
					sqlconf.serviceHost = xml.readResult.value;
				}
				if(xml.readResult.name == "#text" && previous == "serviceport"){
					sqlconf.servicePort = atoi(xml.readResult.value.c_str());
				}

				previous = xml.readResult.name;
			}
			xml.closeReader();
			
			if(!sqlSnake.init(sqlconf.sqlHost, sqlconf.sqlPort, sqlconf.sqlUser, sqlconf.sqlPass, sqlconf.sqlDatabase)){
                                throw MorningException(sqlSnake.getError());
                        }
			sqlSnake.close();
			return true;
		}

		EncryptionSnake fetchPublicKey(EncryptionSnake encryptionSnake){
			encryptionSnake.fetchRsaKeyFromFile(false, false, false, config.pubkey, "");
                        if(encryptionSnake.didFail()){
                                encryptionSnake.cleanOutPublicKey();
                                encryptionSnake.printError();
                                throw MorningException("Failed to load public key.");
                        }
			return encryptionSnake;
		}

		EncryptionSnake fetchPrivateKey(EncryptionSnake encryptionSnake){
			unlockPrivateKey();
                                encryptionSnake.fetchRsaKeyFromFile(true, false, false, config.prikey, algorithms.deriveExportKey(username, password, pin));
                                if(encryptionSnake.didFail()){
                                        encryptionSnake.cleanOutPrivateKey();
                                        encryptionSnake.printError();
                        		lockPrivateKey();
                                        throw MorningException("Failed to load private key.");
                                }
                        lockPrivateKey();
			return encryptionSnake;
		}
};
