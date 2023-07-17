#define MORNING_INBOX_MENU_MAIN 0
#define MORNING_INBOX_MENU_BACK 1
#define MORNING_INBOX_SUB_MAIN 0
#define MORNING_INBOX_SUB_BACK 1
#define MORNING_INBOX_SUB_SEND 2
#define MORNING_INBOX_SUB_UNREAD 3
#define MORNING_INBOX_SUB_READ 4
#define MORNING_INBOX_SUB_REQUEST 5
class MorningInboxtMenu : public MorningMenu{
	private:
		MorningIO io;
		
		FileSnake fileSnake;
		XmlSnake xmlSnake;
		EncryptionSnake encryptionSnake;

		MorningConfig config;
		MorningMessage morningMessage;
		MorningKeyManager morningKey;
		MorningClient client;

		size_t menuCount = 1;
                string menuOptions[1] = {
                        "back"
                };

		size_t subCount = 5;
		string subMenu[5] = {
			"back",
			"send",
			"unread",
			"read",
			"access_request"
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
		friendkey_t *keys = NULL;
		void getUserNames(void){
			if(keys != NULL)
				delete[] keys;
			keys = new friendkey_t[100];
			userNamesCount = morningKey.queryKeys(true, keys, 100);
			if(userNamesCount == 0){
				return;
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

		void setEncryptionSnake(EncryptionSnake es){
			encryptionSnake = es;
			client.setEncryptionSnake(encryptionSnake);
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
				if(userInput == keys[i].alias){
					return i+2;
				}
			}

        	        int inputInt = atoi(getUserInputString().c_str());
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
				io.outf(MORNING_IO_NONE, "%d) %s from %s (%s:%d)\n", i+2, keys[i].alias.c_str(), keys[i].cbhost.c_str(), keys[i].cbip.c_str(), keys[i].cbport);
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

		MorningMenu runMenu(MorningMenu mainMenu){
			int menuCtx = getCoreContext();
                	int subCtx = getSubContext();
                	if(menuCtx == MORNING_INBOX_MENU_MAIN){
                	        if(getShowBanner()){
                	                printBanner();
                	        }
                	        showMenuOptions();
                	        getUserInput();
                	        int ret = parseSelectedOption();
                	        if(ret == -1){
                	                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                	        }else{
                	                setCoreContext(ret);
                	        }
                	}else if(menuCtx == MORNING_INBOX_MENU_BACK){
                	        setShowBanner(true);
                	        mainMenu.setShowBanner(true);
                	        setCoreContext(MORNING_INBOX_MENU_MAIN);
                	        setSubContext(0);
                	        mainMenu.setCoreContext(MORNING_MENU_MAIN);
                	}else if(subCtx == MORNING_INBOX_SUB_MAIN){
                	        showSubMenuOptions();
                	        getUserInput();
                	        int ret = parseSelectedSubOption();
                	        if(ret == -1){
                	                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                	        }else if(ret == MORNING_INBOX_SUB_BACK){
                	                setCoreContext(MORNING_INBOX_MENU_MAIN);
                	        }else{
                	                setSubContext(ret);

                	        }
                	}else if(subCtx == MORNING_INBOX_SUB_SEND){
				string host = keys[menuCtx-2].cbip;
                                if(host == "127.0.0.1")
                                        host = keys[menuCtx-2].cbhost;
                                int port = keys[menuCtx-2].cbport;
				mornmsg message;
        			message.selfDestruct = false;
				io.setIgnore(true);
				io.out(MORNING_IO_GENERAL, "Enter message subject\n");
        			message.subject = io.inWithSpace(MORNING_IO_INPUT, "> ");
				io.out(MORNING_IO_GENERAL, "Enter your message\n");
        			message.message = io.inWithSpace(MORNING_IO_INPUT, "> ");
        			message.messageSize = message.message.length();

				io.out(MORNING_IO_GENERAL, "Sending Message...\n");
				string response = client.sendMessage(host, port, morningMessage.generateSendString(message));
			
				io.outf(MORNING_IO_NONE, "Response Code : %s\n", response.c_str());

				setSubContext(MORNING_INBOX_SUB_MAIN);
			}else if(subCtx == MORNING_INBOX_SUB_UNREAD){
				bool running = true;
				int readCtx = 0;
				mornmsg unreadMessages[200];
				size_t cnt = 0;
				while(running){
					if(readCtx <= 0){
						cnt = morningMessage.getUnreadMessages(keys[menuCtx-2].id, unreadMessages, 200);
						for(int i=0; i<cnt; i++)
							io.outf(MORNING_IO_NONE, "%d) [%s] %s\n", i+1, unreadMessages[i].date.c_str(), unreadMessages[i].subject.c_str());
						io.outf(MORNING_IO_NONE, "%ld) back\n", (long)cnt+1);

						getUserInput();
						string inpt = getUserInputString();
						int inptInt = atoi(inpt.c_str());
						if(inpt == "back" || inptInt == cnt+1){
							break;
						}else if(inptInt >= cnt+2){
							continue;
						}else{
							readCtx = inptInt;
						}
					}else{
						int index = readCtx-1;
						io.outf(MORNING_IO_NONE, "\nDate: %s\nSubject: %s\nMessage: %s\n\n", 
								unreadMessages[index].date.c_str(), 
								unreadMessages[index].subject.c_str(), 
								unreadMessages[index].message.c_str()
						);
						unreadMessages[index].read = true;
						morningMessage.updateMessage(unreadMessages[index]);

						if(!unreadMessages[index].selfDestruct){
							io.outf(MORNING_IO_NONE, "1) keep\n2) delete\n3) unread\n");
							getUserInput();
							string inpt = getUserInputString();
							int inptInt = atoi(inpt.c_str());
							if(inpt == "keep" || inptInt == 1){
								io.out(MORNING_IO_SUCCESS, "Keeping message\n");
								readCtx = 0;
							}else if(inpt == "delete" || inptInt == 2){
								morningMessage.deleteMessage(unreadMessages[index]);
								io.out(MORNING_IO_SUCCESS, "Deleted message\n");
								readCtx = 0;
							}else if(inpt == "unread" || inptInt == 3){
								unreadMessages[index].read = false;
		                                                morningMessage.updateMessage(unreadMessages[index]);
								io.out(MORNING_IO_SUCCESS, "Makred message as not read.\n");
                                                                readCtx = 0;
							}else{
								io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
							}
						}
					}

				}
				setSubContext(MORNING_INBOX_SUB_MAIN);
			}else if(subCtx == MORNING_INBOX_SUB_READ){
				bool running = true;
                                int readCtx = 0;
                                mornmsg readMessages[200];
                                size_t cnt = 0;
                                while(running){
                                        if(readCtx <= 0){
                                                cnt = morningMessage.getReadMessages(keys[menuCtx-2].id, readMessages, 200);
                                                for(int i=0; i<cnt; i++)
                                                        io.outf(MORNING_IO_NONE, "%d) [%s] %s\n", i+1, readMessages[i].date.c_str(), readMessages[i].subject.c_str());
                                                io.outf(MORNING_IO_NONE, "%ld) back\n", (long)cnt+1);

                                                getUserInput();
                                                string inpt = getUserInputString();
                                                int inptInt = atoi(inpt.c_str());
                                                if(inpt == "back" || inptInt == cnt+1){
                                                        break;
                                                }else if(inptInt >= cnt+2){
                                                        continue;
                                                }else{
                                                        readCtx = inptInt;
                                                }
                                        }else{
						int index = readCtx-1;
                                                io.outf(MORNING_IO_NONE, "\nDate: %s\nSubject: %s\nMessage: %s\n\n",
                                                                readMessages[index].date.c_str(),
                                                                readMessages[index].subject.c_str(),
                                                                readMessages[index].message.c_str()
                                                );
						io.outf(MORNING_IO_NONE, "1) keep\n2) delete\n3) unread\n");
						getUserInput();
						string inpt = getUserInputString();
						int inptInt = atoi(inpt.c_str());
						if(inpt == "keep" || inptInt == 1){
							io.out(MORNING_IO_SUCCESS, "Keeping message\n");
							readCtx = 0;
						}else if(inpt == "delete" || inptInt == 2){
							morningMessage.deleteMessage(readMessages[index]);
							io.out(MORNING_IO_SUCCESS, "Deleted message\n");
							readCtx = 0;
						}else if(inpt == "unread" || inptInt == 3){
							readMessages[index].read = false;
		                                        morningMessage.updateMessage(readMessages[index]);
							io.out(MORNING_IO_SUCCESS, "Makred message as not read.\n");
                                                        readCtx = 0;
						}else{
							io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
						}

					}
				}
				setSubContext(MORNING_INBOX_SUB_MAIN);
			}else if(subCtx == MORNING_INBOX_SUB_REQUEST){
				string host = keys[menuCtx-2].cbip;
				if(host == "127.0.0.1")
					host = keys[menuCtx-2].cbhost;
				int port = keys[menuCtx-2].cbport;

				io.outf(MORNING_IO_GENERAL, "Attempting to connect to %s:%d\n", host.c_str(), port);
				if(client.requestAccess(host, port)){
					io.out(MORNING_IO_SUCCESS, "Your key has been received, your friend must approve it before you can chat.\n");
				}else{
					io.out(MORNING_IO_ERROR, "Server is eithr down or it already has your key.\n");
				}
				setSubContext(MORNING_INBOX_SUB_MAIN);
						
			}else if(subCtx > 1){
                	        showMessage();
                	        getUserInput();
                	        int ret = parseSelectedViewOption();
                	        if(ret == -1){
                	                io.out(MORNING_IO_ERROR, "Invalid menu option.\n");
                	        }else if(ret == 3){
                	                setSubContext(0);
                		}else if(ret == 1){
                                	// save message
                       	        	if(saveMessage())
                       	        	        io.out(MORNING_IO_SUCCESS, "Message saved.\n");
                       	        	setSubContext(0);
                       	 	}else if(ret == 2){
                       	         	// delete message
                       	         	if(deleteMessage())
                       	                 	io.out(MORNING_IO_SUCCESS, "Message delete.\n");
                       	         	setSubContext(0);
                       		}
                	}else{
               	         	throw MorningException("Illegal inbox menu context.");
                	}
			return mainMenu;
		}
};
