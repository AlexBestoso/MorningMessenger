class MorningMessenger{
	private:
		const char *verion = "0.2.1 Alpha";
		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;
		MorningMenu menu;
		MorningClientMenu clientMenu;
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
				config.setupMessenger(solicitUsername(), solicitPassword(true), solicitPin());
			}else{
				io.out(MORNING_IO_NONE, "\n\n======== Morning Messenger Login ========\n\n");
				solicitUsername();
                        	solicitPassword(false);
                        	solicitPin();
				config.setSessionCreds(username, password, pin);
			}
			authenticateMessenger();
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

	bool launchServer(void){
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
	
	bool connectToServer(void){
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
		printf("Debug : In manage config file.\n");	
		menu.setCoreContext(MORNING_MENU_MAIN);
		return false;
	}

	void quitMessenger(){
		if(server.lockHeld()){
			server.killProcess();
		}
	}

	int runMainMenu(){
		if(menu.getShowBanner()){
			menu.printBanner();
		}
		menu.showMenuOptions();
		menu.getUserInput();
		int ret = menu.parseSelectedOption();
		if(ret == -1){
			io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
		}else{
			menu.setCoreContext(ret);
		}
		return menu.getCoreContext();
	}

	void authenticateMessenger(void){
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
