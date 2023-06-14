#define MORNING_CONFIG_MENU_MAIN 0
#define MORNING_CONFIG_MENU_BACK 1
#define MORNING_CONFIG_MENU_VIEW 2
#define MORNING_CONFIG_MENU_UPDATE 3

#define MORNING_CONFIG_SUB_BACK 1
#define MORNING_CONFIG_SUB_MESSAGES 2
#define MORNING_CONFIG_SUB_PRIKEY 3
#define MORNING_CONFIG_SUB_PUBKEY 4
#define MORNING_CONFIG_SUB_SERVERHOST 5
#define MORNING_CONFIG_SUB_SERVERPORT 6
#define MORNING_CONFIG_SUB_TRUSTEDKEYS 7

class MorningConfigMenu : public MorningMenu{
	private:
		MorningIO io;
		MorningConfig config;

		EncryptionSnake encryptionSnake;

		size_t menuCount = 3;
                string menuOptions[3] = {
                        "back",
			"view",
			"update"
                };

		size_t subMenuCount = 7;
		string subMenuOptions[7] = {
			"done",
			"messages",
			"prikey",
			"pubkey",
			"serverhost",
			"serverport",
			"trustedkeys"
		};

		void enumerateConfig(void){
			config.loadConfig();
			mornconf cfg = config.getConfig();
			io.outf(MORNING_IO_NONE, "Messages : %s\nPrivate Key : %s\nPublic Key : %s\nServer Host : %s\nServer Port : %d\nTrusted Keys : %s\n\n", 
					cfg.messages.c_str(),
					cfg.prikey.c_str(),
					cfg.pubkey.c_str(),
					cfg.serverhost.c_str(),
					cfg.serverport,
					cfg.trustedkeys.c_str()
			);
		}

		void updateConfig(void){
		
		}
	public:
		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "Config\n";
        	        banner += "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
        	        printf("%s\n", banner.c_str());
        	        setShowBanner(false);
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

		int parseSelectedSubOption(){
                        string userInput = getUserInputString();
                        for(int i=0; i<subMenuCount; i++){
                                if(userInput == subMenuOptions[i]){
                                        return i+1;
                                }
                        }

                        int inputInt = atoi(userInput.c_str());
                        for(int i=0; i<subMenuCount; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }

                        return -1;
                }
                void showSubMenuOptions(){
                        for(int i=0; i<subMenuCount; i++){
                                io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, subMenuOptions[i].c_str());
                        }
                }

		bool runMenu(MorningConfig cfg, EncryptionSnake es){
			config = cfg; 
			encryptionSnake = es;
			int mainCtx = getCoreContext();
			int subCtx = getSubContext();
			if(mainCtx == MORNING_CONFIG_MENU_MAIN){
				if(getShowBanner()){
	                                printBanner();
	                        }
	                        showMenuOptions();
	                        getUserInput();
	                        int ret = parseSelectedOption();
	                        if(ret == -1)
	                                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
	                        else
	                                setCoreContext(ret);
			}else if(mainCtx == MORNING_CONFIG_MENU_VIEW){
				enumerateConfig();
				setCoreContext(MORNING_CONFIG_MENU_MAIN);
			}else if(mainCtx == MORNING_CONFIG_MENU_UPDATE){
				showSubMenuOptions();
                                getUserInput();
                                int ret = parseSelectedSubOption();
				string val = "";
				int val_int = 0;
				mornconf c = config.getConfig();
				switch(ret){
					case MORNING_CONFIG_SUB_BACK:
						setCoreContext(MORNING_CONFIG_MENU_MAIN);
						break;
					case MORNING_CONFIG_SUB_MESSAGES:
						val = io.inString(MORNING_IO_INPUT, "(deprecated) Enter message storage location");
						c.messages = val;
						try{
							config.newConfig(config.getConfigLoc(), c);
							io.out(MORNING_IO_SUCCESS, "Updated message storage location.\n");
						}catch(exception &e){
							io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
						}
						break;
					case MORNING_CONFIG_SUB_PRIKEY:
						val = io.inString(MORNING_IO_INPUT, "Enter private key storage location");
						c.prikey = val;
						try{
                                                        config.newConfig(config.getConfigLoc(), c);
                                                        io.out(MORNING_IO_SUCCESS, "Updated private key location.\n");
                                                }catch(exception &e){
                                                        io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
                                                }
						break;
					case MORNING_CONFIG_SUB_PUBKEY:
						val = io.inString(MORNING_IO_INPUT, "Enter publickey storage location");
						c.pubkey = val;
						try{
                                                        config.newConfig(config.getConfigLoc(), c);
                                                        io.out(MORNING_IO_SUCCESS, "Updated publickey location.\n");
                                                }catch(exception &e){
                                                        io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
                                                }
						break;
					case MORNING_CONFIG_SUB_SERVERHOST:
						val = io.inString(MORNING_IO_INPUT, "Enter server host name");
						c.serverhost = val;
						try{
                                                        config.newConfig(config.getConfigLoc(), c);
                                                        io.out(MORNING_IO_SUCCESS, "Updated server host name.\n");
                                                }catch(exception &e){
                                                        io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
                                                }
						break;
					case MORNING_CONFIG_SUB_SERVERPORT:
						val_int = atoi(io.inString(MORNING_IO_INPUT, "Enter server listen port").c_str());
						c.serverport = val_int;
						try{
                                                        config.newConfig(config.getConfigLoc(), c);
                                                        io.out(MORNING_IO_SUCCESS, "Updated server listen port.\n\tDon't forget to restart the service.\n");
                                                }catch(exception &e){
                                                        io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
                                                }
						break;
					case MORNING_CONFIG_SUB_TRUSTEDKEYS:
						val = io.inString(MORNING_IO_INPUT, "Enter trusted key storage location");
						c.trustedkeys = val;
						try{
                                                        config.newConfig(config.getConfigLoc(), c);
                                                        io.out(MORNING_IO_SUCCESS, "Updated trusted keys location.\n");
                                                }catch(exception &e){
                                                        io.outf(MORNING_IO_FAILURE, "%s\n", e.what());
                                                }
						break;
					default:
						io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
				}
			}else{
				throw MorningException("MorningConfigMenu::runMenu() : Illegal menu option.");
			}
			return true;
		}
};
