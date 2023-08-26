#include "./setup.form.h"
class SetupView : public View{
	private:
		int viewContext = 0;
		SetupViewForm setupForm;
	public:
		morningconfig_t cfg;
		string username = "";
		string password = "";
		bool ready = false;
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			 View::run();
			 switch(viewContext){
			 	default:
					setupForm.run();
					break;
			 }
			 return getViewId();
		}

		int mousePassive(float x, float y){
                        switch(viewContext){
                                default:
                                        setupForm.mousePassive(x, y);
                                        break;
                        }
                        return getViewId();
                }

		int mouseClick(int button, int state, float x, float y){
                        if(viewContext == 0){
				int clickCode = setupForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					switch(setupForm.getClickedButton()){
                                                case 0:
							cfg.sqlHost = setupForm.getSqlHostInput();
                        				cfg.sqlPort = setupForm.getSqlPortInput();
                        				cfg.sqlUser = setupForm.getSqlUserInput();
                        				cfg.sqlPass = setupForm.getSqlPasswordInput();
                        				cfg.sqlDatabase = setupForm.getSqlDatabaseInput();
                        				cfg.serviceHost = setupForm.getServerHostInput();
                        				cfg.servicePort = setupForm.getServerPortInput();
							username = setupForm.getServerUserInput();
							password = setupForm.getServerPasswordInput();
							ready = true;
							break;
					}
				}
			}
			return getViewId();
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
                        switch(viewContext){
                                default:
                                        setupForm.keyDown(key, mouseX, mouseY);
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
