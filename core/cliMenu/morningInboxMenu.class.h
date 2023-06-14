#define MORNING_INBOX_MENU_MAIN 0
#define MORNING_INBOX_MENU_BACK 1
class MorningInboxtMenu : public MorningMenu{
	private:
		MorningIO io;
		
		FileSnake fileSnake;
		XmlSnake xmlSnake;

		MorningConfig config;
		MorningMessage morningMessage;

		size_t menuCount = 1;
                string menuOptions[1] = {
                        "back"
                };

		size_t subCount = 1;
		string subMenu[1] = {
			"back"
		};

		size_t viewCount = 3;
		string viewMenu[3] = {
			"save",
			"delete",
			"back"
		};

		string *userFiles = NULL;
		size_t userFilesCount = 0;
		void getUserFiles(void){
			if(userFiles != NULL)
				delete[] userFiles;
			string loc = config.getTrustedKeysLoc();
			userFiles = fileSnake.listDir(loc);
			userFilesCount = 0;
			while(userFiles[userFilesCount] != ""){
				userFiles[userFilesCount] = loc + "/" + userFiles[userFilesCount];
				userFilesCount++;
			}
		}

		string *userNames = NULL;
		size_t userNamesCount = 0;
		void getUserNames(void){
			getUserFiles();
			if(userFiles == NULL){
				return;
			}
			userNamesCount = userFilesCount;
			if(userNamesCount == 0){
				delete[] userFiles;
				delete[] userNames;
				return;
			}
			if(userNames != NULL)
				delete[] userNames;

			userNames = new string[userNamesCount];
			for(int i=0; i<userNamesCount; i++){
				string target = userFiles[i] + "/user.xml";
				if(!xmlSnake.openFileReader(target)){
					continue;
				}
				string previous = "";
				while(xmlSnake.readLineReader()){
					if(xmlSnake.readResult.name == "#text" && previous == "alias"){
						userNames[i] = xmlSnake.readResult.value;
						break;
					}
					previous = xmlSnake.readResult.name;
				}
				xmlSnake.closeReader();
			}
		}
	
		mornmsg *messages = NULL;
		string *userMessages = NULL;
		size_t userMessagesCount = 0;
		bool getUserMessages(int index){
			userMessagesCount = 0;
			if(index < 0 || index >= userFilesCount)
				return false;
			string messageLocation = userFiles[index] + "/msg";
			if(!fileSnake.fileExists(messageLocation) || fileSnake.getFileType(messageLocation) != FILE_SNAKE_DIR){
				return false;
			}

			if(userMessages != NULL){
				delete[] userMessages;
				userMessages = NULL;
			}

			userMessages = fileSnake.listDir(messageLocation);
			if(userMessages == NULL){
				return false;
			}

			userMessagesCount = 0;
			while(userMessages[userMessagesCount] != ""){
				userMessagesCount++;
			}
			if(userMessagesCount <= 0){
				delete[] userMessages;
				userMessages = NULL;
				return false;
			}

			if(messages != NULL){
				delete[] messages;
				messages = NULL;
			}
			messages = new mornmsg[userMessagesCount];

			for(int i=0; i<userMessagesCount; i++){
				string loc = messageLocation + "/" + userMessages[i];
				messages[i] = morningMessage.getMessageFromFile(loc);
			}

			return true;
		}

	public:

		void setConfig(MorningConfig config){
			this->config = config;
			morningMessage.setConfig(config);
		}

		virtual void printBanner(void){
        	        string banner = "=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=*=+=\n";
			banner += "      _ _|         |                  \n";
			banner += "        |   __ \\   __ \\    _ \\ \\ \\  / \n";
			banner += "        |   |   |  |   |  (   | `  <  \n";
			banner += "      ___| _|  _| _.__/  \\___/  _/\\_\\ \n";
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
			for(int i=0; i<userNamesCount; i++){
				if(userInput == userNames[i]){
					return i+2;
				}
			}

        	        int inputInt = atoi(userInput.c_str());
        	        for(int i=0; i<menuCount; i++){
        	                if(inputInt == i+1){
        	                        return i+1;
        	                }
        	        }
			for(int i=0; i<userNamesCount; i++){
				if(inputInt == i+2){
					return i+2;
				}
			}
	
	                return -1;
	        }
	        virtual void showMenuOptions(){
	                io.outf(MORNING_IO_NONE, "%d) %s\n", 1, menuOptions[0].c_str());
			getUserNames();
			for(int i=0; i<userNamesCount; i++){
				io.outf(MORNING_IO_NONE, "%d) %s\n", i+2, userNames[i].c_str());
			}
	        }

		
		int parseSelectedSubOption(){
                        string userInput = getUserInputString();
                        for(int i=0; i<subCount; i++){
                                if(userInput == subMenu[i]){
                                        return i+1;
                                }
                        }

			int inputInt = atoi(userInput.c_str());
                        for(int i=0; i<subCount; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }
			for(int i=0; i<userMessagesCount; i++){
                                if(inputInt == i+2){
                                        return i+2;
                                }
                        }
			return -1;
		}
		void showSubMenuOptions(){
			int choice = getCoreContext() - 2;
			for(int i=0; i<subCount; i++){
				io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, subMenu[i].c_str());
			}

			if(getUserMessages(choice)){
				for(int i=0; i<userMessagesCount; i++){
					io.outf(MORNING_IO_NONE, "%d) %s | From: %s\n", i+2, messages[i].messageDate.c_str(), messages[i].clientHost.c_str());
				}
			}
		}

		int parseSelectedViewOption(){
			string userInput = getUserInputString();
                        for(int i=0; i<viewCount; i++){
                                if(userInput == viewMenu[i]){
                                        return i+1;
                                }
                        }

                        int inputInt = atoi(userInput.c_str());
                        for(int i=0; i<viewCount; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }
			return -1;
		}
		void showMessage(){
			int choice = getSubContext() - 2;
			int user = getCoreContext() - 2;
			string clientHost = "";
       	 		string messageDate = "";
        		string messageBody = "";
        		string messageLength = "";
			io.outf(MORNING_IO_NONE, "Message From %s(%s) on %s containing %s encrypted bytes\n\n%s\n\n", 
						 userNames[user].c_str(),
						 messages[choice].clientHost.c_str(), 
						 messages[choice].messageDate.c_str(),
						 messages[choice].messageLength.c_str(),
						 messages[choice].messageBody.c_str()
			);
			
			for(int i=0; i<viewCount; i++){
                                io.outf(MORNING_IO_NONE, "%d) %s\n", i+1, viewMenu[i].c_str());
                        }
		}

		bool saveMessage(){
			int user = getCoreContext() - 2;
			int choice = getSubContext() - 2;
			string messageLocation = userFiles[user] + "/msg/" + userMessages[choice];
			string newLocation = userFiles[user] + "/msg_saved";
			if(!fileSnake.fileExists(newLocation)){
				fileSnake.makeDir(newLocation);
			}
			newLocation = newLocation + "/" + userMessages[choice];
			
			size_t fileSize = fileSnake.getFileSize(messageLocation);
			if(fileSize <= 0){
				io.out(MORNING_IO_ERROR, "Failed to save message, files size is 0.\n");
			}
			char *buf = new char[fileSize];
			if(!fileSnake.readFile(messageLocation, buf, fileSize)){
				io.outf(MORNING_IO_ERROR, "Failed to save message, couldn't read message file '%s'\n", messageLocation.c_str());
				return false;
			}
			if(!fileSnake.writeFileTrunc(newLocation, buf, fileSize)){
				io.outf(MORNING_IO_ERROR, "Failed to save message, couldn't write message file '%s'\n", newLocation.c_str());
				return false;
			}
			delete[] buf;
			fileSnake.removeFile(messageLocation);
			return true;
		}

		bool deleteMessage(){
			int user = getCoreContext() - 2;
			int choice = getSubContext() - 2;
			string messageLocation = userFiles[user] + "/msg/" + userMessages[choice];
			fileSnake.removeFile(messageLocation);
			return true;
		}
};
