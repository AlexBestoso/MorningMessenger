#define __REQUEST_VIEW_BACK__ 0
#define __REQUEST__VIEW_SUBMIT__ 1


#include "./request.form.h"
#include "./request.responseForm.h"

class RequestView : public View{
	private:
		RequestViewForm requestForm;
		RequestViewResponseForm responseForm;
		InfoForm infoPopup;
		MorningClient client;

		string networkResponse = "";
		bool firstRun = true;
	public:
		int viewContext = 0;
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			View::run();
			switch(viewContext){
				case 2:
					infoPopup.run();
					break;
				case 1:
					responseForm.run();
					break;
				default:
					requestForm.run();
					break;
			}
			return getViewId();
		}

		int mousePassive(float x, float y){
			switch(viewContext){
				case 2:
					infoPopup.mousePassive(x,y);
					break;
				case 1:
					responseForm.mousePassive(x, y);
					break;
				default:
					requestForm.mousePassive(x, y);
					break;
			}
			return getViewId();
		}
		int mouseClick(int button, int state, float x, float y){
			if(viewContext == 2){
				int clickCode = infoPopup.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode & 0b1) == 1 && (clickCode >> 1) == 0){
					switch(responseForm.getClickedButton()){
						default:
							viewContext = 0;
							firstRun = true;
							return __REQUEST_VIEW_BACK__;
					}
				}
				return -1;
			}
			if(viewContext == 1){
				int clickCode = responseForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					switch(responseForm.getClickedButton()){
						default:
							networkResponse = client.continueAccessRequest(responseForm.getUserInput(), firstRun);
							responseForm.setTextValue(networkResponse);
							if(firstRun){
								responseForm.resetUserInput();
								firstRun = false;
								break;
							}else if(networkResponse != "OK"){
								networkResponse = "The server either failed to process your friend\nrequest or already received a previous request from you.";
								viewContext = 2;
								infoPopup.setTextValue(networkResponse);
								break;
							}
							networkResponse = "The server successfully received your request.\nThey must accept your request before you can chat.\n";
							viewContext = 2;
							infoPopup.setTextValue(networkResponse);
							break;
					}
				}
				return -1;
			}
			int clickCode = requestForm.mouseClick(button, state, x, y);
			if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
				switch(requestForm.getClickedButton()){
					case __REQUEST_VIEW_BACK__:
						return __REQUEST_VIEW_BACK__;
					case __REQUEST__VIEW_SUBMIT__:
						networkResponse = client.startAccessRequest(requestForm.getHostName(), atoi(requestForm.getPort().c_str()));
						responseForm.setTextValue(networkResponse);
						viewContext = 1;
						break;
				}
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			switch(viewContext){
				case 2:

					break;
				case 1:
					responseForm.keyDown(key, mouseX, mouseY);
					break;
				default:
					requestForm.keyDown(key, mouseX, mouseY);
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
