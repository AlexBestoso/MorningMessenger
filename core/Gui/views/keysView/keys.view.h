#include "./keys.form.h"
#include "./keys.untrustedForm.h"
#include "./keys.untrustedManageForm.h"
#include "./keys.trustedForm.h"
#include "./keys.trustedManageForm.h"
class KeysView : public View{
	private:
		int viewContext = 0;
		KeysForm keysForm;
		KeysUntrustedForm untrustedForm;
		KeysTrustedForm trustedForm;
		KeysUntrustedManageForm untrustedManageForm;
		KeysTrustedManageForm trustedManageForm;
		MorningKeyManager keyManager;
		int keyId = -1;
		int returnContext = 0;
		InfoForm popup;
	public:
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			if(viewContext == 0){
				keysForm.run();
			}else if(viewContext == 1){
				trustedForm.run();
			}else if(viewContext == 11){
				trustedManageForm.run();
			}else if(viewContext == 2){
				untrustedForm.run();
			}else if(viewContext == 22){
				untrustedManageForm.run();
			}else if(viewContext == 9){
				popup.run();
			}
			return getViewId();
		}

		int mousePassive(float x, float y){
			if(viewContext == 0){
				keysForm.mousePassive(x, y);
			}else if(viewContext == 1){
				trustedForm.mousePassive(x, y);
			}else if(viewContext == 11){
				trustedManageForm.mousePassive(x, y);
			}else if(viewContext == 2){
				untrustedForm.mousePassive(x, y);
			}else if(viewContext == 22){
				untrustedManageForm.mousePassive(x, y);
                        }else if(viewContext == 9){
				popup.mousePassive(x, y);
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
							trustedForm.updateListButtons();
							break;
						case __KEYS_VIEW__UNTRUSTED:
							viewContext = 2;
							untrustedForm.updateListButtons();
							break;
					}
				}
			}else if(viewContext == 1){
				int clickCode = trustedForm.mouseClick(button, state, x, y);
                                if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
                                        switch(trustedForm.getClickedButton()){
                                                case 0:
                                                        viewContext = 0;
                                                        break;
                                                default:
                                                        keyId = trustedForm.getClickedSelectListOption();
                                                        trustedManageForm.setSelectedKey(trustedForm.getKeyById(keyId));
                                                        keyId = trustedForm.getKeyById(keyId).id;
                                                        viewContext = 11;
                                                        break;
                                        }
                                }
			}else if(viewContext == 11){
				int clickCode = trustedManageForm.mouseClick(button, state, x, y);
                                if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
                                        bool e;
                                        switch(trustedManageForm.getClickedButton()){
                                                case 0:
                                                        viewContext = 1;
                                                        break;
                                                case 1:
                                                        e = keyManager.revokeTrustedKey(keyId);
                                                        returnContext = 1;
                                                        if(e){
                                                                popup.setTextValue("Successfully revoked friend's access.");
                                                        }else{
                                                                popup.setTextValue("Failed to revoke friend's access.");
                                                        }
                                                        trustedForm.updateListButtons();
                                                        viewContext = 9;
                                                        break;
                                                case 2:
                                                        e = keyManager.deleteTrustedKey(keyId);
                                                        returnContext = 1;
                                                        if(e){
                                                                popup.setTextValue("Successfully deleted friend.");
                                                        }else{
                                                                popup.setTextValue("Failed to delete friend.");
                                                        }
                                                        trustedForm.updateListButtons();
                                                        viewContext = 9;
                                                        break;
                                        }
                                }
			}else if(viewContext == 2){
				int clickCode = untrustedForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					switch(untrustedForm.getClickedButton()){
						case 0:
							viewContext = 0;
							break;
						default:
							keyId = untrustedForm.getClickedSelectListOption();
							untrustedManageForm.setSelectedKey(untrustedForm.getKeyById(keyId));
							keyId = untrustedForm.getKeyById(keyId).id;
							viewContext = 22;
							break;
					}
				}
			}else if(viewContext == 22){
				int clickCode = untrustedManageForm.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					bool e;
					switch(untrustedManageForm.getClickedButton()){
						case 0:
							viewContext = 2;
							break;
						case 1:
							e = keyManager.approveUntrustedKey(keyId);
							returnContext = 2;
                                                        if(e){
                                                                popup.setTextValue("Successfully approved friend request.");
                                                        }else{
                                                                popup.setTextValue("Failed to approve friend request.");
                                                        }
                                                        untrustedForm.updateListButtons();
                                                        viewContext = 9;
							break;
						case 2:
							e = keyManager.denyUntrustedKey(keyId);
							returnContext = 2;
							if(e){
								popup.setTextValue("Successfully denied friend request.");
							}else{
								popup.setTextValue("Failed to deny friend request.");
							}
							untrustedForm.updateListButtons();
							viewContext = 9;
							break;
					}
				}
                        }else if(viewContext == 9){
				int clickCode = popup.mouseClick(button, state, x, y);
				if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
					viewContext = returnContext;
				}
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
