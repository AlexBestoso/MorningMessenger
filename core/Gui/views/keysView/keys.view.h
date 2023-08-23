#include "./keys.form.h"
class KeysView : public View{
	private:
		int viewContext = 0;
		KeysForm keysForm;
	public:
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			if(viewContext == 0){
				keysForm.run();
			}else if(viewContext == 1){
			
			}else if(viewContext == 2){
			
			}
			return getViewId();
		}

		int mousePassive(float x, float y){
			if(viewContext == 0){
				keysForm.mousePassive(x, y);
			}else if(viewContext == 1){
			
			}else if(viewContext == 2){
			
			}
                        return getViewId();
                }

		int mouseClick(int button, int state, float x, float y){
			if(viewContext == 0){
				int clickCode = keysForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					switch(keysForm.getClickedButton()){
						case __KEYS_VIEW__BACK:
							viewContext = 0;
							return __KEYS_VIEW__BACK;
						case __KEYS_VIEW__TRUSTED:
							viewContext = 1;
							break;
						case __KEYS_VIEW__UNTRUSTED:
							viewContext = 2;
							break;
					}
				}
			}else if(viewContext == 1){
			
			}else if(viewContext == 2){
			
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			return getViewId();
		}

		int specialKeyDown(int key, int mouseX, int mouseY){return 0;}
                int specialKeyUp(int key, int mouseX, int mouseY){return 0;}
		int keyUp(unsigned char key, int mouseX, int mouseY){return 0;}
		void idle(void){}
		void mouseMovement(void){}
};
