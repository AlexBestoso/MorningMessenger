class KeysTrustedForm : public Form{
	private:
		MorningKeyManager keyManager;

		FormBackground background;
		FormSelectList selectList[1];
		FormButton templateButton;
		FormButton buttons[1];
		keyfile *untrustedKeys = NULL;
                size_t keyCount = 0;
	public:
		KeysTrustedForm() : Form(){
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-keys-trusted.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);
			

			// -- Setup select list
			selectList[0].setMenuWidthHeight(background.getBgW()-0.2, background.getBgH()-0.5);
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
                                (templateButton.getButtonX()+(templateButton.getButtonW()/2))-0.03,
                                templateButton.getButtonY()+(templateButton.getButtonH()/2)-0.01,
                                templateButton.getButtonZ()
                        );
                        templateButton.setButtonText("template");
			updateListButtons();

			// -- back buttons
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
		}

		void updateListButtons(void){
			if(untrustedKeys != NULL){
				delete[] untrustedKeys;
			}
			untrustedKeys = keyManager.fetchTrustedKeys();
                        keyCount = keyManager.trustedKeyCount();
			FormButton *buttons = new FormButton[keyCount];
			
			for(int i=0; i<keyCount; i++){
				string val = to_string(i+1);
				val+= ") "+untrustedKeys[i].alias;
				templateButton.setButtonText(val);
				buttons[i] = templateButton;
			}
			
			selectList[0].setSectionButtonOptions(buttons, keyCount);
			Form::setSelectLists(selectList, 1);
		}

		keyfile getKeyById(int id){
			if(untrustedKeys == NULL || id >= keyCount){
				keyfile ret;
				return ret;
			}
			return untrustedKeys[id];
		}
};
