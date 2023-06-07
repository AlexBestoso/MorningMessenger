#define MORNING_MENU_MAIN 0
#define MORNING_MENU_LAUNCH 1
#define MORNING_MENU_CONNECT 2
#define MORNING_MENU_CONFIG 3
#define MORNING_MENU_MANAGE 4
#define MORNING_MENU_INBOX 5
#define MORNING_MENU_QUIT 6
class MorningMenu{
	private:
		int coreContext = 0;
		int subContext = 0;
		
		bool showBanner = true;

		MorningIO io;
		MorningKeyManager keyManager;

		string userInput = "";
		size_t menuCount = 6;
		string menuOptions[6] = {
			"server",
			"connect",
			"config",
			"manage",
			"inbox",
			"quit"
		};
	public:
		
	virtual void printBanner(void){
		string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
		banner += "   \\  |                   _)                    \\  |                                                   \n";
		banner += "  |\\/ |   _ \\   __| __ \\   |  __ \\    _` |     |\\/ |   _ \\   __|   __|   _ \\  __ \\    _` |   _ \\   __| \n";
		banner += "  |   |  (   | |    |   |  |  |   |  (   |     |   |   __/ \\__ \\ \\__ \\   __/  |   |  (   |   __/  |    \n";
		banner += " _|  _| \\___/ _|   _|  _| _| _|  _| \\__, |    _|  _| \\___| ____/ ____/ \\___| _|  _| \\__, | \\___| _|    \n";
		banner += "                                    |___/                                           |___/              \n";
		banner += "                                          The Bright and Morning Star                                  \n";
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
};
