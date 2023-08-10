class MorningMessenger{
	private:
		const char *version = "0.5.7 Alpha";
		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;
		MorningUser user;

		MorningSetup setup;
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

		bool serverLaunched = false;

	public:
	MorningMessenger(){
		try{
			if(!setup.isSetup()){
				morningconfig_t cfg = setup.runSetup();
				username = cfg.sqlUser;
				password = cfg.sqlPass;
			}else{
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Login ========\n\n");
				username = io.inString(MORNING_IO_INPUT, "Enter username > ");
                                password = io.inString(MORNING_IO_INPUT, "Enter Password > ");
			}

			config.loadConfig();
			if(!user.login(config.getSql(), username, password))
                        	throw MorningException("Authentication Failure : %s", user.getError().c_str());
                        io.out(MORNING_IO_SUCCESS, "Successfully logged in!\n");
			//server.clearLockFile();
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
