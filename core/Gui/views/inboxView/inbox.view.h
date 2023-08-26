#define INBOX_MENU_BACK 0
#include "./inbox.form.h"
#include "./inbox.friendForm.h"
#include "./inbox.unreadForm.h"
#include "./inbox.readForm.h"
class InboxView : public View{
	private:
		InboxViewForm inboxForm;
		InboxViewFriendForm friendForm;
		InboxViewUnreadForm unreadForm;
		InboxViewReadForm readForm;
		RequestViewResponseForm requestForm;
		InfoForm popup;

		MorningClient client;
		MorningMessage morningMessage;

		int friendId = -1;
		int messageId = -1;
		int returnContext = -1;
		bool firstRun = true;
		string networkResponse = "";

		void sendMessage(void){
			friendkey_t val = friendForm.getFriendKey();
			string host = val.cbip;
			if(host == "127.0.0.1")
				host = val.cbhost;
			int port = val.cbport;
			mornmsg message;
			message.selfDestruct = false;
			// TODO: Allow user to enter custom subjects
			message.subject = "Private Message";
			if(message.subject.length() > 50){
				string tmp = "";
				for(int i=0; i<50; i++) 
					tmp += message.subject[i];
				message.subject = tmp;
			}
			message.message = friendForm.getMessage();
			message.messageSize = message.message.length();
			if(message.messageSize <= 0){
				returnContext = 1;
				popup.setTextValue("Message cannot be empty.\n");
				viewContext = 999;
				return;
			}

			try{
				string response = client.sendMessage(host, port, morningMessage.generateSendString(message));
				popup.setTextValue(response);	
				returnContext = 1;
				viewContext = 999;
				friendForm.resetInput();
			}catch(MorningException e){
				string err = "Host : ";
				err += host + ":"+to_string(port)+"\n"+e.what();
				popup.setTextValue(err);
                                returnContext = 1;
                                viewContext = 999;
			}
		}
	public:
		int viewContext = 888;
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			View::run();
			switch(viewContext){
				case 999:
					popup.run();
					break;
				case 1:
					friendForm.run();
					break;
				case 2:
					unreadForm.run();
					break;
				case 3:
					readForm.run();
					break;
				case 4:
					requestForm.run();
					break;
				case 0:
					inboxForm.run();
					break;
				case 888:
					inboxForm.updateListButtons();
					viewContext = 0;
			}
			return getViewId();
		}

		int mousePassive(float x, float y){
			switch(viewContext){
				case 999:
					popup.mousePassive(x, y);
					break;
				case 1:
					friendForm.mousePassive(x, y);
					break;
				case 2:
					unreadForm.mousePassive(x, y);
					break;
				case 3:
					readForm.mousePassive(x, y);
					break;
				case 4:
					requestForm.mousePassive(x, y);
				case 0:
					inboxForm.mousePassive(x, y);
					break;
			}
			return getViewId();
		}
		int mouseClick(int button, int state, float x, float y){
			if(viewContext == 999){
				int clickCode = popup.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode & 0b1) == 1 && (clickCode >> 1) == 0){
					viewContext = returnContext;
				}
				return -1;
			}
			if(viewContext == 2){
				int clickCode = unreadForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode & 0b1) == 1 && (clickCode >> 1) == 0){
					switch(unreadForm.getClickedButton()){
						case 0:
							viewContext = 1;
							messageId = -1;
							break;
						case 1:
							if(messageId >= 0){
								unreadForm.markAsRead(messageId);
								popup.setTextValue("Message has been saved.\n");
								viewContext = 999;
								returnContext = 2;
								messageId = -1;
							}else{
								popup.setTextValue("Please select a message first\n");
                                                                viewContext = 999;
                                                                returnContext = 2;
							}
							break;
						case 2:
							if(messageId >= 0){
								unreadForm.deleteMessage(messageId);
                                                                popup.setTextValue("Message has been deleted.\n");
                                                                viewContext = 999;
                                                                returnContext = 2;
                                                                messageId = -1;
							}else{
								popup.setTextValue("Please select a message first\n");
                                                                viewContext = 999;
                                                                returnContext = 2;
							}
							break;
						default:
							messageId = unreadForm.getClickedSelectListOption();
							unreadForm.displayMessage(messageId);
							
							break;
					}
				}
				return -1;
			}

			if(viewContext == 3){
                                int clickCode = readForm.mouseClick(button, state, x, y);
                                if(clickCode != -1 && (clickCode & 0b1) == 1 && (clickCode >> 1) == 0){
                                        switch(readForm.getClickedButton()){
                                                case 0:
                                                        viewContext = 1;
                                                        messageId = -1;
                                                        break;
                                                case 1:
                                                        if(messageId >= 0){
                                                                readForm.markAsUnread(messageId);
                                                                popup.setTextValue("Message has been marked unread.\n");
                                                                viewContext = 999;
                                                                returnContext = 3;
                                                                messageId = -1;
                                                        }else{
                                                                popup.setTextValue("Please select a message first\n");
                                                                viewContext = 999;
                                                                returnContext = 3;
                                                        }
                                                        break;
                                                case 2:
                                                        if(messageId >= 0){
                                                                readForm.deleteMessage(messageId);
                                                                popup.setTextValue("Message has been deleted.\n");
                                                                viewContext = 999;
                                                                returnContext = 3;
                                                                messageId = -1;
                                                        }else{
                                                                popup.setTextValue("Please select a message first\n");
                                                                viewContext = 999;
                                                                returnContext = 3;
                                                        }
                                                        break;
                                                default:
                                                        messageId = readForm.getClickedSelectListOption();
                                                        readForm.displayMessage(messageId);

                                                        break;
                                        }
                                }
                                return -1;
                        }

			if(viewContext == 4){
                                int clickCode = requestForm.mouseClick(button, state, x, y);
                                if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
                                        switch(requestForm.getClickedButton()){
                                                default:
                                                        networkResponse = client.continueAccessRequest(requestForm.getUserInput(), firstRun);
                                                        requestForm.setTextValue(networkResponse);
                                                        if(firstRun){
                                                                requestForm.resetUserInput();
                                                                firstRun = false;
                                                                break;
                                                        }else if(networkResponse != "OK"){
                                                                networkResponse = "The server either failed to process your friend\nrequest or already received a previous request from you.";
                                                                viewContext = 999;
								returnContext = 1;
                                                                popup.setTextValue(networkResponse);
                                                                break;
                                                        }
							returnContext = 1;
                                                        networkResponse = "The server successfully received your request.\nThey must accept your request before you can chat.\n";
                                                        viewContext = 999;
                                                        popup.setTextValue(networkResponse);
                                                        break;
                                        }
                                }
                                return -1;
                        }


			if(viewContext == 1){
				int clickCode = friendForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode & 0b1) == 1 && (clickCode>>1) == 0){
					switch(friendForm.getClickedButton()){
						case 0:
							viewContext = 0;
							break;
						case 1: // send
							sendMessage();
							break;
						case 2:
							unreadForm.setFriendKeyId(inboxForm.getFriendKeyById(friendId).id);
							unreadForm.updateListButtons();
							viewContext = 2;
							break;
						case 3:
							readForm.setFriendKeyId(inboxForm.getFriendKeyById(friendId).id);
                                                        readForm.updateListButtons();
                                                        viewContext = 3;
                                                        break;
						case 4:
							networkResponse = client.startAccessRequest(inboxForm.getFriendKeyById(friendId).cbhost, inboxForm.getFriendKeyById(friendId).cbport);
                                                	requestForm.setTextValue(networkResponse);
							viewContext = 4;
							break;
					}
				}
				return -1;
			}
			int clickCode = inboxForm.mouseClick(button, state, x, y);
			if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
				switch(inboxForm.getClickedButton()){
					case 0:
						inboxForm.updateListButtons();
						viewContext = 888;
						return INBOX_MENU_BACK;
					default:
						friendId = inboxForm.getClickedSelectListOption();
						friendForm.setFriendKey(inboxForm.getFriendKeyById(friendId));
						viewContext = 1;
				}
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			switch(viewContext){
				case 4:
					requestForm.keyDown(key, mouseX, mouseY);
				case 1:
					friendForm.keyDown(key, mouseX, mouseY);
					break;
				case 0:
					inboxForm.keyDown(key, mouseX, mouseY);
					break;
			}
			return getViewId();
		}
		int specialKeyDown(int key, int mouseX, int mouseY){return 0;}
                int specialKeyUp(int key, int mouseX, int mouseY){return 0;}
                int keyUp(unsigned char key, int mouseX, int mouseY){return 0;}
                void idle(void){}
                void mouseMovement(void){}
};
