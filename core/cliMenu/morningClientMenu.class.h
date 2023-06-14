#define MORNING_CLIENT_MENU_MAIN 0
#define MORNING_CLIENT_MENU_STANDARD 1
#define MORNING_CLIENT_MENU_LIVE 2
#define MORNING_CLIENT_MENU_BACK 3
class MorningClientMenu : public MorningMenu{
	private:
		MorningIO io;
		size_t menuCount = 3;
                string menuOptions[3] = {
                        "standard",
                        "live",
                        "back"
                };
	public:
		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "             __|  |            |       \\  |                   \n";
			banner += "            (       \\    _` |   _|    |\\/ |   -_)    \\   |  | \n";
			banner += "           \\___| _| _| \\__,_| \\__|   _|  _| \\___| _| _| \\_,_| \n";
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
};
