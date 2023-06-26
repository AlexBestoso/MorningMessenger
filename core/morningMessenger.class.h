class MorningMessenger{
	private:
		const char *version = "0.4.11 Alpha";
		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;
		MorningUser user;

		MorningMenu menu;
		MorningClientMenu clientMenu;
		MorningManagerMenu managerMenu;
		MorningInboxtMenu inboxMenu;
	//	MorningConfigMenu configMenu;
		MorningServerMenu serverMenu;
		
		MorningServer server;
		MorningClient client;

		FileSnake fileSnake;
		EncryptionSnake encryptionSnake;

		string username = "";
		string password = "";
		string pin = "";

		bool serverLaunched = false;

		string solicitUsername(void){
			username = io.inString(MORNING_IO_INPUT, "Enter username > ");
			return username;
		}

		string solicitPassword(bool ensureCorrect){
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

			return password;
		}
		string solicitPin(void){
			pin = io.inString(MORNING_IO_INPUT, "Enter Pin Number > ");
			return pin;
		}
	public:
	MorningMessenger(){
		try{
			if(!fileSnake.fileExists(config.getConfigLoc())){
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Setup ========\n\n");
				morningconfig_t cfg;
				cfg.sqlHost = io.inString(MORNING_IO_INPUT, "Enter MySql server hostname / ip > ");
				cfg.sqlPort = (unsigned int)atoi(io.inString(MORNING_IO_INPUT, "Enter MySql port (3306) > ").c_str());
				cfg.sqlUser = io.inString(MORNING_IO_INPUT, "Enter MySql username > ");
				cfg.sqlPass = io.inString(MORNING_IO_INPUT, "Enter MySql password > ");
				cfg.sqlDatabase = io.inString(MORNING_IO_INPUT, "Enter your desired database name (mornmessenger) > ");
				cfg.serviceHost = io.inString(MORNING_IO_INPUT, "Enter messenger service hostname (127.0.0.1) > ");
				cfg.servicePort = atoi(io.inString(MORNING_IO_INPUT, "Enter messenger service port (21345) > ").c_str());
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
				config.loadConfig();
				if(!user.login(config.getSql(), username, password))
					throw MorningException("Authentication Failure : %s", user.getError().c_str());

				io.out(MORNING_IO_SUCCESS, "Successfully logged in!\n");
			}else{
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Login ========\n\n");
				config.loadConfig();
				string username = solicitUsername();
                                string password = solicitPassword(false);
				 if(!user.login(config.getSql(), username, password))
                                        throw MorningException("Authentication Failure : %s", user.getError().c_str());

                                io.out(MORNING_IO_SUCCESS, "Successfully logged in!\n");
			}
			server.clearLockFile();
		}catch(exception &e){
			string what = e.what();
			what = "Failed to start up Morning Messenger.\nCaught in MorningMessenger::MorningMessenger() | " + what;
			throw MorningException(what);
		}
	}

	int getMenuContext(void){
		return menu.getCoreContext();
	}

	bool findFriends(void){
		menu.setCoreContext(MORNING_MENU_MAIN);
		client.setConfig(config);
		client.setEncryptionSnake(encryptionSnake);
		io.out(MORNING_IO_GENERAL, "Enter the host name / IP address and the port number of the server you want to request access to.\n");
		string ip = io.inString(MORNING_IO_INPUT, "Enter Host Name > ");
                int port = atoi(io.inString(MORNING_IO_INPUT, "Enter Port Number (Default : 21345) > ").c_str());
		if(!client.requestAccess(ip, port)){
			io.out(MORNING_IO_ERROR, "The server failed to store your approval request.\n");
                	return false;
		}else{
			io.out(MORNING_IO_SUCCESS, "Your message has been received and is awaiting approval.\n");
                        return true;
		}
	}

/*	Keeping this function to keep track of how I was sending messages, for the time being.
 *
 *	__attribute__((deprecated("connecToServer is getting replaced."))) bool connectToServer(void){
		int menuCtx = clientMenu.getCoreContext();
		if(menuCtx == MORNING_CLIENT_MENU_MAIN){
			if(clientMenu.getShowBanner()){
				clientMenu.printBanner();
			}
			clientMenu.showMenuOptions();
			clientMenu.getUserInput();
			int ret = clientMenu.parseSelectedOption();
			if(ret == -1){
				io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
			}else{
				clientMenu.setCoreContext(ret);
			}
		}else if(menuCtx == MORNING_CLIENT_MENU_BACK){
			clientMenu.setShowBanner(true);
			menu.setShowBanner(true);
                        clientMenu.setCoreContext(MORNING_CLIENT_MENU_MAIN);
                        menu.setCoreContext(MORNING_MENU_MAIN);
		}else if(menuCtx == MORNING_CLIENT_MENU_STANDARD){
			client.setConfig(config);
			client.setEncryptionSnake(encryptionSnake);
			
			if(!client.connectToServer()){
				io.out(MORNING_IO_ERROR, "Failed to connect to client.\n");
			}

			clientMenu.setCoreContext(MORNING_CLIENT_MENU_BACK);
		}else if(menuCtx == MORNING_CLIENT_MENU_LIVE){
			printf("DEBUG | In live chat menu");
			clientMenu.setCoreContext(MORNING_CLIENT_MENU_BACK);
		}else{
			throw MorningException("Illegal chat menu context.\n");
		}
		return true;
	}*/

	/*bool manageConfigFile(void){
		if(configMenu.getCoreContext() == MORNING_CONFIG_MENU_BACK){
			config.loadConfig();
			configMenu.setShowBanner(true);
                        menu.setShowBanner(true);
                        configMenu.setCoreContext(MORNING_CONFIG_MENU_MAIN);
                        configMenu.setSubContext(0);
                        menu.setCoreContext(MORNING_MENU_MAIN);
			return true;
		}
		
		return configMenu.runMenu(config, encryptionSnake);
	}*/
	
	bool keyManager(void){
		int menuCtx = managerMenu.getCoreContext();
		int subCtx = managerMenu.getSubContext();
		if(menuCtx == MORNING_MANAGER_MENU_MAIN){
			if(managerMenu.getShowBanner()){
                                managerMenu.printBanner();
                        }
                        managerMenu.showMenuOptions();
                        managerMenu.getUserInput();
                        int ret = managerMenu.parseSelectedOption();
                        if(ret == -1){
                                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                        }else{
                                managerMenu.setCoreContext(ret);
                        }
		}else if(menuCtx == MORNING_MANAGER_MENU_BACK){
			managerMenu.setShowBanner(true);
			menu.setShowBanner(true);
			managerMenu.setCoreContext(MORNING_MANAGER_MENU_MAIN);
			managerMenu.setSubContext(0);
			menu.setCoreContext(MORNING_MENU_MAIN);
		}else if(menuCtx == MORNING_MANAGER_MENU_UNTRUSTED && subCtx == 0){
			managerMenu.showUntrustedKeyOptions();
			if(managerMenu.getCoreContext() == MORNING_MANAGER_MENU_MAIN){
				managerMenu.setSubContext(0);
				return true;
			}

		}else if(menuCtx == MORNING_MANAGER_MENU_UNTRUSTED && subCtx > 0){
			return managerMenu.manageUntrustedKey();
		}else if(menuCtx == MORNING_MANAGER_MENU_TRUSTED && subCtx == 0){
			managerMenu.showTrustedKeyOptions();
                        if(managerMenu.getCoreContext() == MORNING_MANAGER_MENU_MAIN){
                                managerMenu.setSubContext(0);
                                return true;
                        }
		}else if(menuCtx == MORNING_MANAGER_MENU_TRUSTED && subCtx > 0){
			return managerMenu.manageTrustedKey();
		}else{
                        throw MorningException("Illegal manager menu context. Option ID %d\n", menuCtx);
                }
		return true;
	}

	bool inbox(void){
		inboxMenu.setEncryptionSnake(encryptionSnake);
		menu = inboxMenu.runMenu(menu);
		return true;
	}

	void quitMessenger(void){
		if(server.lockHeld()){
			server.killProcess();
		}
	}

	void runServerMenu(void){
		menu = serverMenu.runMenu(menu, config.getSql());
	}

	bool runMainMenu(){
		return menu.runMenu(version);
	}
};
