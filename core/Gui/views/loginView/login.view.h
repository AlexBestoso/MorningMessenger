#include "./login.form.h"
class LoginView : public View{
	private:
		int viewContext = 0;
		LoginViewForm loginForm;
		
	public:
		string username = "";
		string password = "";
		bool ready = false;
		bool authFailed = false;

		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			if(authFailed){
				loginForm.hideAuthError(false);
			}
			View::run();
			loginForm.run();
			return getViewId();
		}

		int mousePassive(float x, float y){
			loginForm.mousePassive(x, y);
			return getViewId();
		}

		int mouseClick(int button, int state, float x, float y){
			int clickCode = loginForm.mouseClick(button, state, x, y);
			if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
				switch(loginForm.getClickedButton()){
					case 0:
						ready = true;
						username = loginForm.getUsernameInput();
						password = loginForm.getPasswordInput();
						break;
				}
			}
			loginForm.setAccessability(false);
			return getViewId();
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			if(key == 0x09){ // accessability using the tab key.
				if(!loginForm.getAccessability()){
                                        loginForm.setAccessability(true);
                                        loginForm.setAccessabilityElement(0);
                                }else{
                                        loginForm.accessabilityNext();
                                }

				return -1;
			}
			if(loginForm.getAccessability() && loginForm.getAccessabilityId() == 2 && key == 0xd){
				// Pressed enter using accessibility to login.
				ready = true;
                                username = loginForm.getUsernameInput();
                                password = loginForm.getPasswordInput();
				return -1;
			}
			loginForm.keyDown(key, mouseX, mouseY);
			return getViewId();
		}


		int specialKeyDown(int key, int mouseX, int mouseY){return 0;}
                int specialKeyUp(int key, int mouseX, int mouseY){
			if(key == 0x66 || key == 0x67){
				// right arrow or down arrow
				if(!loginForm.getAccessability()){
					loginForm.setAccessability(true);
					loginForm.setAccessabilityElement(0);
				}else{
					loginForm.accessabilityNext();
				}
			}else if(key == 0x64 || key == 0x65){
				// up arrow or left arrow
				if(!loginForm.getAccessability()){
					loginForm.setAccessability(true);
                                        loginForm.setAccessabilityElement(999);
                                }else{
                                        loginForm.accessabilityPrevious();
                                }
			}
			return 0;
		}
		int keyUp(unsigned char key, int mouseX, int mouseY){return 0;}
		void idle(void){}
		void mouseMovement(void){}

		void resetInputs(void){
			loginForm.resetInputs();
		}
};
