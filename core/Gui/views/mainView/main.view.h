#define __MAIN_VIEW__SERVERCTRL 0
#define __MAIN_VIEW__FINDFRIENDS 1
#define __MAIN_VIEW__MANAGEKEYS 2
#define __MAIN_VIEW__INBOX 3
#define __MAIN_VIEW__LOGOUT 4

#include "./main.form.h"

class MainView : public View{
	private:
		int viewContext = 0;
		MainViewForm mainForm;

	public:
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			 View::run();
			 mainForm.run();
			 return getViewId();
		}

		int mousePassive(float x, float y){
			mainForm.mousePassive(x, y);
                        return getViewId();
                }

		int mouseClick(int button, int state, float x, float y){
			int clickCode = mainForm.mouseClick(button, state, x, y);
			if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
				switch(mainForm.getClickedButton()){
					case __MAIN_VIEW__SERVERCTRL:
						return __MAIN_VIEW__SERVERCTRL;
					case __MAIN_VIEW__FINDFRIENDS:
						return __MAIN_VIEW__FINDFRIENDS;
					case __MAIN_VIEW__MANAGEKEYS:
						return __MAIN_VIEW__MANAGEKEYS;
					case __MAIN_VIEW__INBOX:
						return __MAIN_VIEW__INBOX;
					case __MAIN_VIEW__LOGOUT:
						return __MAIN_VIEW__LOGOUT;
				}
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			mainForm.keyDown(key, mouseX, mouseY);
			return getViewId();
		}

		int specialKeyDown(int key, int mouseX, int mouseY){return 0;}
                int specialKeyUp(int key, int mouseX, int mouseY){return 0;}
		int keyUp(unsigned char key, int mouseX, int mouseY){return 0;}
		void idle(void){}
		void mouseMovement(void){}
};
