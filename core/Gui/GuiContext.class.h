#define GUI_CONTEXT_SETUP 1
#define GUI_CONTEXT_LOGIN 2

#include "./views/viewLinker.h"

class GuiContext{
	private:
		int current_context = 0;
                float bgColor[4] = {0.25, 0.0, 0.5, 1};
		MorningMessenger morningMessenger;
		SetupView setupView;

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
					printf("Login view.\n");
					break;
				default:
					if(morningMessenger.isSetup()){
						current_context = GUI_CONTEXT_LOGIN;
					}else{
						current_context = GUI_CONTEXT_SETUP;
					}
					break;
			}

		}

		void mouseClickContextSwitch(int button, int state, int x, int y){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mouseClick(button, state, x, y);
					if(setupView.ready){
						setupView.ready = false;
						morningMessenger.runSetup(setupView.cfg, setupView.username, setupView.password);
						current_context = GUI_CONTEXT_LOGIN;
					}
                                        break;
			}
		}

		void mouseMovementContextSwitch(void){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mouseMovement();
                                        break;
                        }
		}

		void mousePassiveContextSwitch(float x, float y){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.mousePassive(x, y);
                                        break;
                        }
		}

		void idleContextSwitch(void){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.idle();
                                        break;
                        }
		}

		void keydownContextSwitch(unsigned char key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.keyDown(key, mouseX, mouseY);
                                        break;
                        }
		}

		void keyupContextSwitch(unsigned char key, int mouseX, int mouseY){
			switch(current_context){
                                case GUI_CONTEXT_SETUP:
					setupView.keyUp(key, mouseX, mouseY);
                                        break;
                        }
		}
}context;
