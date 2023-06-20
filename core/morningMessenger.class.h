class MorningMessenger{
	private:
		const char *version = "0.4.7 Alpha";
		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;
		MorningUser user;

		MorningMenu menu;
		MorningClientMenu clientMenu;
		MorningManagerMenu managerMenu;
		MorningInboxtMenu inboxMenu;
		MorningConfigMenu configMenu;
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

				io.out(MORNING_IO_GENERAL, "Logging into your new account!\n");
				if(!user.login(config.getSql(), username, password))
					throw MorningException("Authentication Failure : %s", user.getError());

				io.out(MORNING_IO_SUCCESS, "Successfully logged in!\n");
			}else{
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Login ========\n\n");
				config.loadConfig();
				string username = solicitUsername();
                                string password = solicitPassword(false);
				 if(!user.login(config.getSql(), username, password))
                                        throw MorningException("Authentication Failure : %s", user.getError());

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

	__attribute__((deprecated("Obsolete function will be removed in future versions")))bool launchServer(void){
		if(server.obtainLockFile()){
			server.pid = fork();
			if(server.pid == 0){
				/* Launch the server */
				server.setConfig(config);
				server.setEncryptionSnake(encryptionSnake);
				try{
					server.launchServer();
				}catch(exception &e){
					server.clearLockFile();
					string what = e.what();
					what = "MorningMessenger::launchServer | Failure From Child Process.\n" + what;
					throw MorningException(what);
				}
			}else{
				io.out(MORNING_IO_SUCCESS, "Server has been started.\n");
			}
                }else{
			string choice = io.inString(MORNING_IO_QUESTION, "Would you lke to stop the server? > ");
			if(choice == "yes"){
				server.killProcess();
				io.out(MORNING_IO_SUCCESS, "Server has been stopped.\n");
			}
                }
		menu.setCoreContext(MORNING_MENU_MAIN);
		return true;
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

	__attribute__((deprecated("connecToServer is getting replaced."))) bool connectToServer(void){
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
	}

	bool manageConfigFile(void){
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
	}
	
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
		}else if(menuCtx == MORNING_MANAGER_MENU_TRUSTED){
			throw MorningException("Trusted key management has not been programmed yet");
		}else{
                        throw MorningException("Illegal manager menu context. Option ID %d\n", menuCtx);
                }
		return true;
	}

	bool inbox(void){
		int menuCtx = inboxMenu.getCoreContext();
                int subCtx = inboxMenu.getSubContext();
		inboxMenu.setConfig(config);
                if(menuCtx == MORNING_INBOX_MENU_MAIN){
			if(inboxMenu.getShowBanner()){
                                inboxMenu.printBanner();
                        }
                        inboxMenu.showMenuOptions();
                        inboxMenu.getUserInput();
                        int ret = inboxMenu.parseSelectedOption();
                        if(ret == -1){
                                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                        }else{
                                inboxMenu.setCoreContext(ret);
                        }
		}else if(menuCtx == MORNING_INBOX_MENU_BACK){
                        inboxMenu.setShowBanner(true);
                        menu.setShowBanner(true);
                        inboxMenu.setCoreContext(MORNING_INBOX_MENU_MAIN);
                        inboxMenu.setSubContext(0);
                        menu.setCoreContext(MORNING_MENU_MAIN);
                }else if(subCtx == 0){
			inboxMenu.showSubMenuOptions();
                        inboxMenu.getUserInput();
			int ret = inboxMenu.parseSelectedSubOption();
			if(ret == -1){
                                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                        }else if(ret == 1){
				inboxMenu.setCoreContext(MORNING_INBOX_MENU_MAIN);
			}else{
                                inboxMenu.setSubContext(ret);
                        }
		}else if(subCtx > 1){
			inboxMenu.showMessage();
			inboxMenu.getUserInput();
			int ret = inboxMenu.parseSelectedViewOption();
			if(ret == -1){
                                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                        }else if(ret == 3){
                                inboxMenu.setSubContext(0);
			}else if(ret == 1){
				// save message
				if(inboxMenu.saveMessage())
					io.out(MORNING_IO_SUCCESS, "Message saved.\n");
                                inboxMenu.setSubContext(0);
			}else if(ret == 2){
				// delete message
				if(inboxMenu.deleteMessage())
					io.out(MORNING_IO_SUCCESS, "Message delete.\n");
                                inboxMenu.setSubContext(0);
			}
		}else{
			throw MorningException("Illegal inbox menu context.");
		}
		return true;
	}

	void quitMessenger(void){
		if(server.lockHeld()){
			server.killProcess();
		}
	}

	void runServerMenu(void){
		menu = serverMenu.runMenu(menu);
	}

	bool runMainMenu(){
		return menu.runMenu(version);
	}

	__attribute__((depricated("Obsolete function will be removed in future versions.")))void authenticateMessenger(void){
		try{
			config.loadConfig();	
			io.out(MORNING_IO_SUCCESS, "Configuration loaded.\n");
			mornconf c = config.getConfig();
			
			io.outf(MORNING_IO_GENERAL, "Loading public key '%s'\n", c.pubkey.c_str());
			config.fetchPublicKey(encryptionSnake);
			
			io.outf(MORNING_IO_GENERAL, "Unlocking and loading private key from '%s'\n", c.prikey.c_str());
			encryptionSnake = config.fetchPrivateKey(encryptionSnake);
			io.outf(MORNING_IO_SUCCESS, "Welcome Home, %s. You're authenticated.\n\n\n", username.c_str());

		}catch(exception &e){
			string what = e.what();
			what = "Authentication failure\nCaugth in MorningMessenger::authenticateMessenger() | " + what ;
			throw MorningException(what);
		}
	}

};
