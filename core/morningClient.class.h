class MorningClient{
	private:
		NetSnake netSnake;
                FileSnake fileSnake;
                EncryptionSnake encryptionSnake;
		EncryptionSnake remoteCerts;
                MorningConfig config;
		MorningIO io;

                string cmd_newUser = "a235210cd5476dfa0a045d60244e5cc6aebbc21e406fba344ce5d154b6337f5b";
                string cmd_existingUser = "f0773273589a87e67e71f402c08c93b464ff307846de3e7567dc1e423d65baf9";
                string cmd_streamChat = "6c0df3e03ccc8a63b41937acc7169cabad337da6bc7bb36a19245548d6eef2a3";
                string cmd_chat = "7b5a3431691d02a5f335235bae96e65301311115d3688e2306c0a664ab29bfbe";

		size_t levelOneCommandCount = 2;
		string levelOneCommands[2] = {
			"request_new",
			"connect"
		};

		int parseSelectedOption(string input, string *options, size_t len){
                        for(int i=0; i<len; i++){
                                if(input == options[i]){
                                        return i+1;
                                }
                        }

                        int inputInt = atoi(input.c_str());
                        for(int i=0; i<len; i++){
                                if(inputInt == i+1){
                                        return i+1;
                                }
                        }

                        return -1;
                }

		bool validateResponse(){
			char *okay = new char[2];
                         if(!netSnake.recvInetClient(okay, 2, 0)){
                         	netSnake.closeSocket();
                                io.out(MORNING_IO_ERROR, "Failed to recv command validation\n");
                                return false;
                         }
                         string okayString = "";
			 for(int i=0; i<netSnake.recvSize; i++)
				 okayString += okay[i];
			 printf("Recevied : %s\n", okayString.c_str());
			 if(okayString == "OK"){
			 	return true;
			 }
			 return false;
		}

		int getLevelOne(void){
			while(1){
				for(int i=0; i<levelOneCommandCount; i++){
					printf("%d) %s\n", i+1, levelOneCommands[i].c_str());
				}
				string input = io.inString(MORNING_IO_INPUT, " > ");
				int option = parseSelectedOption(input, levelOneCommands, levelOneCommandCount);
				if(option == -1){
					io.out(MORNING_IO_ERROR, "Invalid option.\n");
				}else if(option == 1){
					return 1;
				}else if(option == 2){
					return 2;
				}else{
					return -1;
				}
			}
			return -1;
		}

		bool sendPublicKey(){
			return true;
		}

		bool recvPublicKey(){
			return true;
		}

		bool sendAccessRequest(){
			if(!netSnake.sendInetClient(cmd_newUser.c_str(), cmd_newUser.length())){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to send command to remote server\n");
				return false;
			}

			if(!validateResponse()){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to validate command response.\n");
				return false;
			}
			return true;
		}

		bool sendAuthRequest(){
			if(!netSnake.sendInetClient(cmd_existingUser.c_str(), cmd_existingUser.length())){
				netSnake.closeSocket();
				io.out(MORNING_IO_ERROR, "Failed to send command to remote server\n");
				return false;
			}

			if(!validateResponse()){
				netSnake.closeSocket();
				return false;
			}
			return true;
		}

	public:

		void setConfig(MorningConfig config){
                	this->config = config;
        	}
        	void setEncryptionSnake(EncryptionSnake encryptionSnake){
        	        this->encryptionSnake = encryptionSnake;
        	}
		
		bool connectToServer(void){
			string ip = io.inString(MORNING_IO_INPUT, "Enter Host Name > ");
			int port = atoi(io.inString(MORNING_IO_INPUT, "Enter Port Number > ").c_str());
			if(!netSnake.createClient(ip, port, 0)){
				return false;
			}
			
			int levelOne = getLevelOne();
			if(levelOne == -1){
				netSnake.closeSocket();
				return false;
			}else if(levelOne == 1){ // Access Request Protocol
				if(!sendAccessRequest()){
					return false;
				}

				if(!sendPublicKey()){
					netSnake.closeSocket();
					return false;
				}

				if(!recvPublicKey()){
					netSnake.closeSocket();
					return false;
				}

			}else if(levelOne == 2){ // Try to authenticate with remote server
				if(!sendAuthRequest()){
					return false;
				}

				if(!sendPublicKey()){
                                        netSnake.closeSocket();
                                        return false;
                                }

                                if(!recvPublicKey()){
                                        netSnake.closeSocket();
                                        return false;
                                }
			}else{
				io.out(MORNING_IO_ERROR, "Invalid command attempted.\n");
			}
			netSnake.closeSocket();
			return true;
		}
};
