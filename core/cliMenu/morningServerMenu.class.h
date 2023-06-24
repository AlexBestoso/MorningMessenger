#define MORNING_SERVER_MENU_MAIN 0
#define MORNING_SERVER_MENU_CONFIG 1
#define MORNING_SERVER_MENU_START 2
#define MORNING_SERVER_MENU_STOP 3
#define MORNING_SERVER_MENU_STATUS 4
#define MORNING_SERVER_MENU_BACK 5
class MorningServerMenu : public MorningMenu{
	private:
		MorningIO io;
		MorningServer server;
		size_t menuCount = 5;
                string menuOptions[5] = {
                        "config",
                        "start",
			"stop",
			"status",
                        "back"
                };

		void handlConfigMenu(void){
			string subOptions[5] = {
				"newkeys",
        			"serverName",
				"hostname",
				"port",
				"back"
			};

			for(int i=0; i<5; i++)
                                io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, subOptions[i].c_str());
			
			getUserInput();
			int newCtx = parseSubOptions(subOptions, 5);
			if(newCtx == -1){
				io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
				return;
			}
			if(newCtx == 5){
				setCoreContext(MORNING_SERVER_MENU_MAIN);
				setSubContext(0);
				return;
			}
			setSubContext(newCtx);
		}

		void processNewPort(){
			io.out(MORNING_IO_NONE, "Enter A new port number\n");
			getUserInput();
			serverconfig_t cfg = server.getServerConfig();
			cfg.serverPort = atoi(getUserInputString().c_str());
			server.updateConfig(cfg);
			io.out(MORNING_IO_SUCCESS, "Updated server port.\n");
			setSubContext(0);
		}
		void processNewHost(){
			io.out(MORNING_IO_NONE, "Enter A new server host name\n\n");
                        getUserInput();
                        serverconfig_t cfg = server.getServerConfig();
                        cfg.serverHost = getUserInputString();
                        server.updateConfig(cfg);
                        io.out(MORNING_IO_SUCCESS, "Updated server host name\n");	
			setSubContext(0);
		}
		void processNewServerName(){
			io.out(MORNING_IO_NONE, "Enter A new server alias name\n\n");
                        getUserInput();
                        serverconfig_t cfg = server.getServerConfig();
                        cfg.serverName = getUserInputString();
                        server.updateConfig(cfg);
                        io.outf(MORNING_IO_SUCCESS, "Updated server alias name to '%s'\n", cfg.serverName.c_str());
			setSubContext(0);
		}
		void processNewKeyPair(){
			io.out(MORNING_IO_NONE, "Generating new key pair, please be patient...\n");
			server.updateConfig(server.generateNewKeyPair());
                        io.outf(MORNING_IO_SUCCESS, "Updated server key pair\n");
			setSubContext(0);
		}

		void startService(){
			io.out(MORNING_IO_GENERAL, "Starting Service...\n");
			if(fork() == 0){
				char *args[] = {
					"/usr/sbin/service",
					"morningService",
					"start",
					NULL
				};
				execvp(args[0], args);
				exit(0);
			}
		}
		void stopService(){
			io.out(MORNING_IO_GENERAL, "Stopping Service...\n");    
                        if(fork() == 0){
                                char *args[] = {
                                        "/usr/sbin/service",
					"morningService",
                                        "stop",
                                        NULL
                                };
                                execvp(args[0], args);
				exit(0);
                        }
		}
		void statusService(){
			io.out(MORNING_IO_GENERAL, "Getting Service Status...\n");    
                        if(fork() == 0){
                                char *args[] = {
                                        "/usr/sbin/service",
					"morningService",
                                        "status",
                                        NULL
                                };
                                execvp(args[0], args);
				exit(0);
                        }
		}
	public:
		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "Server\n";
        	        banner += "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
        	        printf("%s\n", banner.c_str());
        	        setShowBanner(false);
        	}

		int parseSubOptions(string *opts, size_t optsSize){
			string userInput = getUserInputString();
                        for(int i=0; i<optsSize; i++){
                                if(userInput == opts[i]){
                                        return i+1;
                                }
                        }

                        int inputInt = atoi(userInput.c_str());
                        for(int i=0; i<optsSize; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }

                        return -1;
		}
		virtual int parseSelectedOption(){
			string userInput = getUserInputString();
        	        for(int i=0; i<menuCount; i++){
        	                if(userInput == menuOptions[i]){
        	                        return i+1;
        	                }
        	        }

        	        int inputInt = atoi(userInput.c_str());
        	        for(int i=0; i<menuCount; i++){
        	                if(inputInt == i+1){
        	                        return i+1;
        	                }
        	        }
	
	                return -1;
	        }
	        virtual void showMenuOptions(){
	                for(int i=0; i<menuCount; i++){
	                        io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, menuOptions[i].c_str());
	                }
	        }

		MorningMenu runMenu(MorningMenu mainMenu, SqlSnake sqlSnake){
			int ctx = getCoreContext();
			int stx = getSubContext();
			serverconfig_t cfg;
			switch(ctx){
				case MORNING_SERVER_MENU_MAIN:
					if(getShowBanner()){
                			        printBanner();
                			}
                			showMenuOptions();
                			getUserInput();
                			ctx = parseSelectedOption();
                			if(ctx == -1){
                			        io.out(MORNING_IO_ERROR, "Invalid Menu Option\n");
                			        return mainMenu;
                			}
                			setCoreContext(ctx);
					break;
				case MORNING_SERVER_MENU_CONFIG:
					server.loadConfigs();
					cfg = server.getServerConfig();
					if(stx == 0){
						io.outf(MORNING_IO_NONE, "server host : %s:%d\n\nPublic Key :\n%s\n\nServer Name: %s\n\n", 
								cfg.serverHost.c_str(), cfg.serverPort, cfg.publicKey.c_str(), cfg.serverName.c_str());
						handlConfigMenu();
					}else if(stx == 1){
						processNewKeyPair();
					}else if(stx == 2){
						processNewServerName();
					}else if(stx == 3){
						processNewHost();
					}else if(stx == 4){
						processNewPort();
					}else{
						io.outf(MORNING_IO_ERROR, "Invalid menu option.\n");
						setCoreContext(MORNING_SERVER_MENU_MAIN);
						setSubContext(0);
					}
					break;
				case MORNING_SERVER_MENU_START:
					startService();
					setCoreContext(0);
					break;
				case MORNING_SERVER_MENU_STOP:
					stopService();
					setCoreContext(0);
					break;
				case MORNING_SERVER_MENU_STATUS:
					statusService();
					setCoreContext(0);
					break;
				case MORNING_SERVER_MENU_BACK:
					setShowBanner(true);
                        		mainMenu.setShowBanner(true);
                        		setCoreContext(MORNING_SERVER_MENU_MAIN);
                        		setSubContext(0);
                        		mainMenu.setCoreContext(MORNING_MENU_MAIN);
					break;
				default:
					throw MorningException("Illegal menu context in server menu.");
			}
               	 	return mainMenu;
		}
};
