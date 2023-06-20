#define MORNING_SERVER_MENU_MAIN 0
#define MORNING_SERVER_MENU_CONFIG 1
#define MORNING_SERVER_MENU_START 2
#define MORNING_SERVER_MENU_STOP 3
#define MORNING_SERVER_MENU_STATUS 4
#define MORNING_SERVER_MENU_BACK 5
class MorningServerMenu : public MorningMenu{
	private:
		MorningIO io;
		size_t menuCount = 5;
                string menuOptions[5] = {
                        "config",
                        "start",
			"stop",
			"status",
                        "back"
                };
	public:
		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "Server\n";
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

		MorningMenu runMenu(MorningMenu mainMenu){
			int ctx = getCoreContext();
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
					// TODO : build out and test key loading before moving forward.	
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
