#define MORNING_MANAGER_MENU_MAIN 0
#define MORNING_MANAGER_MENU_UNTRUSTED 1
#define MORNING_MANAGER_MENU_TRUSTED 2
#define MORNING_MANAGER_MENU_BACK 3
class MorningManagerMenu : public MorningMenu{
	private:
		MorningIO io;
		MorningKeyManager keyManager;

		size_t menuCount = 3;
                string menuOptions[3] = {
                        "untrusted",
                        "trusted",
                        "back"
                };

		keyfile *untrustedKeys = NULL;
	public:
		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "  |  /                    \\  |                                                            |   \n";
			banner += "  ' /    _ \\  |   |      |\\/ |   _` |  __ \\    _` |   _` |   _ \\  __ `__ \\    _ \\  __ \\   __| \n";
			banner += "  . \\    __/  |   |      |   |  (   |  |   |  (   |  (   |   __/  |   |   |   __/  |   |  |   \n";
			banner += " _|\\_\\ \\___| \\__, |     _|  _| \\__,_| _|  _| \\__,_| \\__, | \\___| _|  _|  _| \\___| _|  _| \\__| \n";
			banner += "             ____/                                  |___/                                     \n";
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

		bool manageUntrustedKey(){
			int subContext = getSubContext();
			if(subContext == 0 || untrustedKeys == NULL || subContext-1 >= keyManager.untrustedKeyCount() || keyManager.untrustedKeyCount() <= 0){
				io.out(MORNING_IO_ERROR, "Invalid untrusted key ID.\n");
				setCoreContext(MORNING_MANAGER_MENU_MAIN);
				return false;
			}
			subContext--;

			io.outf(MORNING_IO_NONE, "Alias : %s\nMessage: %s\nPublic Key -\n%s\n", untrustedKeys[subContext].alias.c_str(),
												untrustedKeys[subContext].message.c_str(),
												untrustedKeys[subContext].publickey.c_str()
			);
			bool inputValidating = true;
			int inputInt = 0;
			string inpt = "";
			while(inputValidating){
				inpt = io.inString(MORNING_IO_NONE, "1) approve\n2) deny\n3) cancel\n > ");
				inputInt = atoi(inpt.c_str());
				if((inpt == "approve" || inputInt == 1) || 
				   (inpt == "deny" || inputInt == 2) || 
				   (inpt == "cancel" || inputInt == 3)){
					break;
				}else{
					io.out(MORNING_IO_ERROR, "Invalid input\n");
				}
			}

			if(inpt == "cancel" || inputInt == 3){
				setSubContext(0);
				setCoreContext(MORNING_MANAGER_MENU_MAIN);
                                return true;
			}else if(inpt == "approve" || inputInt == 1){
				bool ret = keyManager.approveUntrustedKey(subContext);
				setCoreContext(MORNING_MANAGER_MENU_MAIN);
				setSubContext(0);
				if(!ret)
					io.outf(MORNING_IO_ERROR, "Failed to approve key for '%s'\n", untrustedKeys[subContext].alias.c_str());
				else
					io.outf(MORNING_IO_SUCCESS, "Approved key for '%s'\n", untrustedKeys[subContext].alias.c_str());
				delete[] untrustedKeys;
				return ret;
			}else{
				bool ret = keyManager.denyUntrustedKey(subContext);
				setCoreContext(MORNING_MANAGER_MENU_MAIN);
				setSubContext(0);
				if(!ret)
					io.outf(MORNING_IO_ERROR, "Failed to deny key for '%s'\n", untrustedKeys[subContext].alias.c_str());
				else
					io.outf(MORNING_IO_SUCCESS, "Denied key for '%s'\n", untrustedKeys[subContext].alias.c_str());
				delete[] untrustedKeys;
				return ret;
			}
			return false;
		}

		void showUntrustedKeyOptions(void){
			untrustedKeys = keyManager.fetchUntrustedKeys();
			int keyCount = keyManager.untrustedKeyCount();
			for(int i=0; i<keyCount; i++){
				io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, untrustedKeys[i].alias.c_str());
			}
			io.outf(MORNING_IO_NONE, "%d) quit\n", keyCount+1);

			getUserInput();

			string userInput = getUserInputString();
			if(userInput == "quit"){
                        	setCoreContext(MORNING_MANAGER_MENU_MAIN);
                                return;
                        }
                        for(int i=0; i<keyCount; i++){
                                if(userInput == untrustedKeys[i].alias){
					setSubContext(i+1);
                                        return;
                                }
                        }

                        int inputInt = atoi(userInput.c_str());
			if(inputInt == keyCount+1){
				setCoreContext(MORNING_MANAGER_MENU_MAIN);
				delete[] untrustedKeys;
                                return;
                        }
                        for(int i=0; i<keyCount; i++){
                                if(inputInt == i+1){
                                        setSubContext(i+1);
                                        return;
                                }
                        }
		}
};
