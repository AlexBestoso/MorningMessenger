class MorningSetup{
	private:
		MorningConfig config;
		MorningServer server;
		MorningUser user;
		MorningIO io;

		FileSnake fileSnake;

		string solicitUsername(void){
                       	return io.inString(MORNING_IO_INPUT, "Enter username > ");
                }

                string solicitPassword(bool ensureCorrect){
			string password;
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
                        	return password;
                        }else{
                                return io.inString(MORNING_IO_INPUT, "Enter Password > ");
                        }
                }

	public:
		bool isSetup(void){
			if(fileSnake.fileExists(config.getConfigLoc()))
				return true;
			return false;
		}

		morningconfig_t runSetup(void){
			morningconfig_t cfg;
			cfg.sqlHost = io.inString(MORNING_IO_INPUT, "Enter MySql server hostname / ip > ");
			cfg.sqlPort = (unsigned int)atoi(io.inString(MORNING_IO_INPUT, "Enter MySql port (3306) > ").c_str());
			cfg.sqlUser = io.inString(MORNING_IO_INPUT, "Enter MySql username > ");
			cfg.sqlPass = io.inString(MORNING_IO_INPUT, "Enter MySql password > ");
			cfg.sqlDatabase = io.inString(MORNING_IO_INPUT, "Enter your desired database name (mornmessenger) > ");
			cfg.serviceHost = "127.0.0.1";
			cfg.servicePort = 21345;
			config.setupMessenger(cfg);

			io.out(MORNING_IO_GENERAL, "Adding new user.\n");
			string username = solicitUsername();
			string password = solicitPassword(true);
			if(!user.newUser(config.getSql(), username, password))
				throw MorningException(user.getError());
			io.out(MORNING_IO_SUCCESS, "User created successfully!\n");

			io.out(MORNING_IO_GENERAL, "Generating server key pair. Please be patient.\n");
			server.setupServerTable(config.getSql());
			io.out(MORNING_IO_SUCCESS, "Server table successfully set up!\n");

			io.out(MORNING_IO_GENERAL, "Setting up trusted keys table.\n");
			MorningKeyManager keyManager;
			keyManager.setupTable();
			io.out(MORNING_IO_SUCCESS, "Trusted key table created!\n");

			io.out(MORNING_IO_GENERAL, "Setting up group table.\n");
			MorningGroupManager groupManager;
			groupManager.setupTable();
			io.out(MORNING_IO_SUCCESS, "Created group table!\n");

			io.out(MORNING_IO_GENERAL, "Setting up message table.\n");
			MorningMessage mornMessage;
			mornMessage.setupTable();
			io.out(MORNING_IO_SUCCESS, "Created message table!\n");

			io.out(MORNING_IO_GENERAL, "Logging into your new account!\n");

			/*
			 * This is hacky, should fix it in the future
			 * */
			cfg.sqlPass = password;
			cfg.sqlUser = username;
			return cfg;
		}
};
