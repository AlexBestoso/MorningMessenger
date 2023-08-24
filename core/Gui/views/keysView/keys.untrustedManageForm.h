class KeysUntrustedManageForm : public Form{
	private:
		MorningKeyManager keyManager;

		FormBackground background;
		FormButton buttons[3];
		FormScrollText scrollText[1];

		keyfile *untrustedKeys = NULL;
                size_t keyCount = 0;
	public:
		KeysUntrustedManageForm() : Form(){
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-keys-untrusted.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);
			
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

			// -- Approve Button
			buttons[1].setWidthHeight(.17, 0.09);
                        buttons[1].setCoords(
                                background.getBgX()+0.2,
                                background.getBgY()+0.03,
                                0
                        );
                        buttons[1].setButtonColor(.2, .2, .2);
                        buttons[1].setButtonTextCoords(
                                (buttons[1].getButtonX()+(buttons[1].getButtonW()/2))-0.03,
                                buttons[1].getButtonY()+(buttons[1].getButtonH()/2)-0.01,
                                buttons[1].getButtonZ()
                        );
                        buttons[1].setButtonText("Approve");
			
			// -- Deny Button
			buttons[2].setWidthHeight(.17, 0.09);
                        buttons[2].setCoords(
                                buttons[1].getButtonX() + buttons[1].getButtonW() + 0.03,
                                background.getBgY()+0.03,
                                0
                        );
                        buttons[2].setButtonColor(.2, .2, .2);
                        buttons[2].setButtonTextCoords(
                                (buttons[2].getButtonX()+(buttons[2].getButtonW()/2))-0.03,
                                buttons[2].getButtonY()+(buttons[2].getButtonH()/2)-0.01,
                                buttons[2].getButtonZ()
                        );
                        buttons[2].setButtonText("Deny");

			Form::setButtons(buttons, 3);

			// -- Key Data Display
                        scrollText[0].setMenuColor(.5, .5, .5);
                        scrollText[0].setMenuWidthHeight(background.getBgW(), 0.7);
                        scrollText[0].setMenuCoords(
                                background.getBgX(),
                                background.getBgY()+background.getBgH()-scrollText[0].getMenuH(),
                                0
                        );
			scrollText[0].setTextFont(6);
                        scrollText[0].setEscapeText(false);
                        scrollText[0].setMaxCols(73);
			scrollText[0].setMaxRows(15);
                        scrollText[0].setText("tmp");
                        scrollText[0].setTextCoords(
				scrollText[0].getMenuX()+0.03,
				scrollText[0].getMenuY()+scrollText[0].getMenuH()-0.04,
                                0
                        );

                        Form::setScrollTexts(scrollText, 1);
		}

		void setSelectedKey(keyfile val){
			if(untrustedKeys != NULL){
				delete[] untrustedKeys;
			}
			untrustedKeys = new keyfile[1];
			untrustedKeys[0] = val;

			string txt = "ID : ";
			txt += to_string(untrustedKeys[0].id) + "\n";
			txt += "Alias : "+untrustedKeys[0].alias + "\n";
			txt += "Message : "+untrustedKeys[0].message + "\n";
			txt += "Public Key : \n"+untrustedKeys[0].publickey + "\n";

			scrollText[0].setText(txt);
		}

		int getKeyId(void){
			if(untrustedKeys != NULL)
				return untrustedKeys[0].id;
			printf("Returning error.\n");
			return -1;
		}
};
