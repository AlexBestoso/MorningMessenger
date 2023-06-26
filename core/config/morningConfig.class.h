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

		const char *storageLocation = "/var/morningService/MMS_Storage";
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
			loadConfig();
			return sqlSnake;
		}

		string getConfigLoc(){
			return getConfigLoc(false);
		}
		__attribute__((deprecated("Bool is being removed in future versions")))string getConfigLoc(bool serverMode){
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
				sqlconf = cfg;
				generateConfig(cfg);
				setupDatabase();
				setupServerKeysDir();

			}catch(exception &e){
				string what = e.what();
                                what = "Faild to setup Morning Messenger.\nCaught in MorningMessenger::setupMessenger() | " + what;
                                throw MorningException(what);
			}	
		}

		bool generateConfig(morningconfig_t config){
			string fileName = getConfigLoc(false);
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

		bool loadConfig(){
			string fileName = getConfigLoc();
			if(!xml.openFileReader(fileName)){
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
};
