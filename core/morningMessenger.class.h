class MorningMessenger{
	private:
		const char *verion = "0.0.0 Alpha";
		MorningIO io;
		MorningAlgorithms algorithms;
		MorningConfig config;
		MorningMenu menu;

		NetSnake netsnake;
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
		printf("Debug : In launch server.\n");
		menu.setCoreContext(MORNING_MENU_MAIN);
		return false;
	}
	
	bool connectToServer(void){
		printf("Debug : In connect to server.\n");	
		menu.setCoreContext(MORNING_MENU_MAIN);
		return false;
	}

	bool manageConfigFile(void){
		printf("Debug : In manage config file.\n");	
		menu.setCoreContext(MORNING_MENU_MAIN);
		return false;
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
			config.fetchPrivateKey(encryptionSnake);
			io.outf(MORNING_IO_SUCCESS, "Welcome Home, %s. You're authenticated.\n\n\n", username.c_str());

		}catch(exception &e){
			string what = e.what();
			what = "Authentication failure\nCaugth in MorningMessenger::authenticateMessenger() | " + what ;
			throw MorningException(what);
		}
	}

};
