/*
 * This class is not meant to be used in the main application, nor is it designed to have
 * a definite functionality. It's here so that we can hook into the main context switch 
 * and allow us to test new and experimental graphical components.
 * */
#include "./debug.form.h"

class DebugView : public View{
	private:
		int viewContext = 0;
		DebugForm debugForm;
	public:
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			 View::run();
			 debugForm.run();
			 return getViewId();
		}

		int mousePassive(float x, float y){
			debugForm.mousePassive(x, y);
                        return getViewId();
                }

		int mouseClick(int button, int state, float x, float y){
			int clickCode = debugForm.mouseClick(button, state, x, y);
			if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
				switch(debugForm.getClickedButton()){
				}
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			debugForm.keyDown(key, mouseX, mouseY);
			return getViewId();
		}

		int specialKeyDown(int key, int mouseX, int mouseY){
			debugForm.specialKeyDown(key, mouseX, mouseY);
			return 0;
		}
                int specialKeyUp(int key, int mouseX, int mouseY){
			debugForm.specialKeyUp(key, mouseX, mouseY);
			return 0;
		}
		int keyUp(unsigned char key, int mouseX, int mouseY){
			debugForm.keyUp(key, mouseX, mouseY);
			return 0;
		}
		void idle(void){
			debugForm.idle();
		}
		void mouseMovement(void){
			debugForm.idle();
		}
};
