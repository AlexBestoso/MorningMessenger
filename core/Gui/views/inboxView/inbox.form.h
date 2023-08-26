class InboxViewForm : public Form{
	private:
		FormBackground background;

		MorningKeyManager morningKey;

		string *userNames = NULL;
                size_t userNamesCount = 0;
                friendkey_t *keys = NULL;
                void getUserNames(void){
                        if(keys != NULL)
                                delete[] keys;
                        keys = new friendkey_t[100];
                        userNamesCount = morningKey.queryKeys(true, keys, 100);
                }

		FormButton buttons[1];
		FormButton templateButton;
		FormSelectList selectList[1];
		MorningSetup setup;
	public:
		InboxViewForm() : Form(){
			if(!setup.isSetup()){
                                return;
                        }
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-inbox.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// -- Setup back button.
			buttons[0].setWidthHeight(.17, 0.09);
                        buttons[0].setCoords(
                                background.getBgX()+background.getBgW()-buttons[0].getButtonW()-0.03,
                                background.getBgY()+0.03,
                                0
                        );
                        buttons[0].setButtonColor(.2, .2, .2);
                        buttons[0].setButtonTextCoords(
                                (buttons[0].getButtonX()+(buttons[0].getButtonW()/2))-0.03,
                                buttons[0].getButtonY()+(buttons[0].getButtonH()/2)-0.01,
                                buttons[0].getButtonZ()
                        );
                        buttons[0].setButtonText("Back");
                        Form::setButtons(buttons, 1);

			// -- Setup select list
                        selectList[0].setMenuWidthHeight(background.getBgW()-0.1, background.getBgH()-0.5);
                        selectList[0].setMenuCoords(
                                background.getBgX() + 0.1,
                                background.getBgY()+(background.getBgW()/4),
                                background.getBgZ()
                        );
                        selectList[0].setMenuColor(.3, .3, .3);
                        selectList[0].setMaxVisibleOptions(4);
                        selectList[0].setOptionsOffset(0.03);

                        // -- Setup template Button
                        templateButton.setWidthHeight(selectList[0].getMenuW(), 0.09);
                        templateButton.setCoords(
                                        selectList[0].getMenuX(),
                                        selectList[0].getMenuY()+selectList[0].getMenuH() - templateButton.getButtonH(),
                                        0
                        );
                        templateButton.setButtonColor(.2, .2, .2);
                        templateButton.setButtonTextCoords(
                                templateButton.getButtonX(),
                                templateButton.getButtonY()+(templateButton.getButtonH()/2)-0.01,
                                templateButton.getButtonZ()
                        );
                        templateButton.setButtonText("");
                        updateListButtons();
		}

		void updateListButtons(void){
			getUserNames();
			if(userNamesCount == 0){
				selectList[0].setSectionButtonOptions(NULL, 0);
                        	Form::setSelectLists(selectList, 1);
				return;
			}
                        FormButton *buttons = new FormButton[userNamesCount];

                        for(int i=0; i<userNamesCount; i++){
                                string val = to_string(i+1);
				val += ") "+keys[i].alias + " from ";
			       	val += keys[i].cbhost + "("; 
				val += keys[i].cbip + ":" + to_string(keys[i].cbport) + ")";
                                templateButton.setButtonText(val);
                                buttons[i] = templateButton;
                        }

                        selectList[0].setSectionButtonOptions(buttons, userNamesCount);
                        Form::setSelectLists(selectList, 1);
                }

		friendkey_t getFriendKeyById(int id){
			return keys[id];
		}
};
