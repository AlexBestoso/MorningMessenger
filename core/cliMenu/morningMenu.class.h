#define MORNING_MENU_MAIN 0
#define MORNING_MENU_LAUNCH 1
#define MORNING_MENU_FINDFRIENDS 2
#define MORNING_MENU_MANAGE 3
#define MORNING_MENU_INBOX 4
#define MORNING_MENU_QUIT 5
class MorningMenu{
	private:
		int coreContext = 0;
		int subContext = 0;
		
		bool showBanner = true;

		MorningIO io;
		MorningKeyManager keyManager;

		string userInput = "";
		size_t menuCount = 5;
		string menuOptions[5] = {
			"serverctrl",
			"findFriends",
			"manage",
			"inbox",
			"quit"
		};
	public:
		
	virtual void printBanner(string version){
		string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
		banner += "   \\  |                   _)                    \\  |                                                   \n";
		banner += "  |\\/ |   _ \\   __| __ \\   |  __ \\    _` |     |\\/ |   _ \\   __|   __|   _ \\  __ \\    _` |   _ \\   __| \n";
		banner += "  |   |  (   | |    |   |  |  |   |  (   |     |   |   __/ \\__ \\ \\__ \\   __/  |   |  (   |   __/  |    \n";
		banner += " _|  _| \\___/ _|   _|  _| _| _|  _| \\__, |    _|  _| \\___| ____/ ____/ \\___| _|  _| \\__, | \\___| _|    \n";
		banner += "                                    |___/                                           |___/              \n";
		banner += "                                          The Bright and Morning Star                                  \n";
		banner += "                                             "+version+"                                               \n";
		banner += "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
		printf("%s\n", banner.c_str());
		showBanner = false;
	}

	virtual void info(){
		int count = keyManager.untrustedKeyCount();
		if(count > 0){
			if(count == 1)
				io.outf(MORNING_IO_GENERAL, "\tThere is 1 untrusted key\n");
			else
				io.outf(MORNING_IO_GENERAL, "\tThere are %d untrusted keys\n", count);
		}
	}

	string getUserInputString(void){
		return userInput;
	}

	bool getShowBanner(void){
		return showBanner;
	}
	void setShowBanner(bool v){
		showBanner = v;
	}

	int getCoreContext(void){
		return coreContext;
	}
	void setCoreContext(int ctx){
		coreContext = ctx;
	}

	int getSubContext(void){
		return subContext;
	}
	void setSubContext(int ctx){
		subContext = ctx;
	}
	
	void getUserInput(){
		userInput = io.inString(MORNING_IO_INPUT, " > ");
	}

	virtual int parseSelectedOption(){
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
	virtual bool runMenu(string version){
		if(getShowBanner()){
			printBanner(version);
		}
		info();
		showMenuOptions();
		getUserInput();
		int ret = parseSelectedOption();
		if(ret == -1){
			io.out(MORNING_IO_ERROR, "Invalid Menu Option\n");
			return false;
		}
		setCoreContext(ret);
		return true;
	}
};
