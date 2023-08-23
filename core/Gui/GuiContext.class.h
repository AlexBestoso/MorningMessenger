#define GUI_CONTEXT_SETUP 1
#define GUI_CONTEXT_LOGIN 2
#define GUI_CONTEXT_MAIN 3
#define GUI_CONTEXT_SERVER 4
#define GUI_CONTEXT_FRIENDS 5
#define GUI_CONTEXT_KEYS 6
#define GUI_CONTEXT_INBOX 7

#include "./views/viewLinker.h"

class GuiContext{
	private:
		int current_context = 0;
                float bgColor[4] = {0.25, 0.0, 0.5, 1};
		MorningMessenger morningMessenger;
		SetupView setupView;
		LoginView loginView;
		MainView mainView;
		ServerView serverView;
		RequestView requestView;
		KeysView keysView;
	public:
		int displayW = 0;
		int displayH = 0;
		void setContext(int newContext){
                        current_context = newContext;
                }

		void displayContextSwitch(void){
			switch(current_context){
				case GUI_CONTEXT_SETUP:
					if(!setupView.isInit()){
						setupView.init(0, bgColor);
					}
					setupView.run();
					break;
				case GUI_CONTEXT_LOGIN:
					if(!loginView.isInit()){
						loginView.init(0, bgColor);
					}
					loginView.run();
					if(loginView.ready){
                                                loginView.ready = false;
                                                loginView.authFailed = false;
                                                if(!morningMessenger.login(loginView.username, loginView.password)){
                                                        loginView.authFailed = true;
                                                }else{
                                                        current_context = GUI_CONTEXT_MAIN;
                                                        loginView.resetInputs();
                                                }
                                        }
					break;
				case GUI_CONTEXT_MAIN:
					if(!mainView.isInit()){
						mainView.init(0, bgColor);
					}
					mainView.run();
					break;
				case GUI_CONTEXT_SERVER:
					if(!serverView.isInit()){
						serverView.init(0, bgColor);
					}
					serverView.run();
					break;
				case GUI_CONTEXT_FRIENDS:
					if(!requestView.isInit())
						requestView.init(0, bgColor);
					requestView.run();
					break;
				case GUI_CONTEXT_KEYS:
					if(!keysView.isInit()){
						keysView.init(0, bgColor);
					}
					keysView.run();
					break;
				default:
					if(morningMessenger.isSetup()){
						current_context = GUI_CONTEXT_LOGIN;
					}else{
						current_context = GUI_CONTEXT_SETUP;
					}
					//debug
				//	current_context = GUI_CONTEXT_SERVER;
					break;
			}

		}

		void mouseClickContextSwitch(int button, int state, int x, int y){
			int clickEvent = -1;
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mouseClick(button, state, x, y);
					if(setupView.ready){
						setupView.ready = false;
						morningMessenger.runSetup(setupView.cfg, setupView.username, setupView.password);
						current_context = GUI_CONTEXT_LOGIN;
					}
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.mouseClick(button, state, x, y);
					break;
				case GUI_CONTEXT_MAIN:
					clickEvent = mainView.mouseClick(button, state, x, y);
					if(clickEvent != -1){
						if(clickEvent == __MAIN_VIEW__LOGOUT){
							loginView.username = "";
							loginView.password = "";
							current_context = GUI_CONTEXT_LOGIN;
						}else if(clickEvent == __MAIN_VIEW__SERVERCTRL){
							current_context = GUI_CONTEXT_SERVER;
						}else if(clickEvent == __MAIN_VIEW__FINDFRIENDS){
							current_context = GUI_CONTEXT_FRIENDS;
						}else if(clickEvent == __MAIN_VIEW__MANAGEKEYS){
							current_context = GUI_CONTEXT_KEYS;
						}
					}
					break;
				case GUI_CONTEXT_SERVER:
					clickEvent = serverView.mouseClick(button, state, x, y);
					if(serverView.viewContext == __SERVER_MAIN__){
						if(clickEvent == __SERVER_BACK__){
							current_context = GUI_CONTEXT_MAIN;
							break;
						}
					}
					break;
				case GUI_CONTEXT_FRIENDS:
					clickEvent = requestView.mouseClick(button, state, x, y);
					if(clickEvent == __REQUEST_VIEW_BACK__){
						current_context = GUI_CONTEXT_MAIN;
					}
					break;
				case GUI_CONTEXT_KEYS:
					clickEvent = keysView.mouseClick(button, state, x, y);
					if(clickEvent == __KEYS_VIEW__BACK){
						current_context = GUI_CONTEXT_MAIN;
					}	
					break;
			}
		}

		void mouseMovementContextSwitch(void){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mouseMovement();
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.mouseMovement();
					break;
				case GUI_CONTEXT_MAIN:
					mainView.mouseMovement();
					break;
				case GUI_CONTEXT_SERVER:
					serverView.mouseMovement();
					break;
				case GUI_CONTEXT_FRIENDS:
					requestView.mouseMovement();
					break;
				case GUI_CONTEXT_KEYS:
					keysView.mouseMovement();
					break;
                        }
		}

		void mousePassiveContextSwitch(float x, float y){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mousePassive(x, y);
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.mousePassive(x, y);
					break;
				case GUI_CONTEXT_MAIN:
					mainView.mousePassive(x, y);
					break;
				case GUI_CONTEXT_SERVER:
					serverView.mousePassive(x, y);
					break;
				case GUI_CONTEXT_FRIENDS:
					requestView.mousePassive(x, y);
					break;
				case GUI_CONTEXT_KEYS:
					keysView.mousePassive(x, y);
					break;
                        }
		}

		void idleContextSwitch(void){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.idle();
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.idle();
					break;
				case GUI_CONTEXT_MAIN:
					mainView.idle();
					break;
				case GUI_CONTEXT_SERVER:
					serverView.idle();
					break;
				case GUI_CONTEXT_FRIENDS:
					requestView.idle();
					break;
				case GUI_CONTEXT_KEYS:
					keysView.idle();
					break;
                        }
		}

		void keydownContextSwitch(unsigned char key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.keyDown(key, mouseX, mouseY);
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.keyDown(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_MAIN:
					mainView.keyDown(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_SERVER:
					serverView.keyDown(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_FRIENDS:
					requestView.keyDown(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_KEYS:
					keysView.keyDown(key, mouseX, mouseY);
					break;
                        }
		}
		void specialKeydownContextSwitch(int key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
                                        setupView.specialKeyDown(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_LOGIN:
                                        loginView.specialKeyDown(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_MAIN:
                                        mainView.specialKeyDown(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_SERVER:
                                        serverView.specialKeyDown(key, mouseX, mouseY);
                                        break;
				case GUI_CONTEXT_FRIENDS:
					requestView.specialKeyDown(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_KEYS:
					keysView.specialKeyDown(key, mouseX, mouseY);
					break;
                        }
		}
		void specialKeyupContextSwitch(int key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
                                        setupView.specialKeyUp(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_LOGIN:
                                        loginView.specialKeyUp(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_MAIN:
                                        mainView.specialKeyUp(key, mouseX, mouseY);
                                        break;
                                case GUI_CONTEXT_SERVER:
                                        serverView.specialKeyUp(key, mouseX, mouseY);
                                        break;
				case GUI_CONTEXT_FRIENDS:
					requestView.specialKeyUp(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_KEYS:
					keysView.specialKeyUp(key, mouseX, mouseY);
					break;
                        }
		}

		void keyupContextSwitch(unsigned char key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.keyUp(key, mouseX, mouseY);
                                        break;
				case GUI_CONTEXT_LOGIN:
					loginView.keyUp(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_MAIN:
					mainView.keyUp(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_SERVER:
					serverView.keyUp(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_FRIENDS:
					requestView.keyUp(key, mouseX, mouseY);
					break;
				case GUI_CONTEXT_KEYS:
					keysView.keyUp(key, mouseX, mouseY);
					break;
                        }
		}
}context;
