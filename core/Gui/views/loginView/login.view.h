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
			return getViewId();
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			loginForm.keyDown(key, mouseX, mouseY);
			return getViewId();
		}

		int keyUp(unsigned char key, int mouseX, int mouseY){
			return 0;
		}

		void idle(void){}
		void mouseMovement(void){}

};
