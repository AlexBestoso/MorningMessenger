class InboxViewFriendForm : public Form{
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

		int butt_back = 0;
		int butt_send = 1;
		int butt_unread = 2;
		int butt_read = 3;
		int butt_requestAccess = 4;
		size_t buttonCount = 5;
		FormButton buttons[5];

		FormTextInput textInputs[1];

		FormText texts[1];

		friendkey_t friendKey;

	public:
		InboxViewFriendForm() : Form(){
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-inbox.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// -- Setup back button.
			buttons[butt_back].setWidthHeight(.17, 0.09);
                        buttons[butt_back].setCoords(
                                background.getBgX()+background.getBgW()-buttons[butt_back].getButtonW()-0.03,
                                background.getBgY()+0.03,
                                0
                        );
                        buttons[butt_back].setButtonColor(.2, .2, .2);
                        buttons[butt_back].setButtonTextCoords(
                                (buttons[butt_back].getButtonX()+(buttons[butt_back].getButtonW()/2))-0.03,
                                buttons[butt_back].getButtonY()+(buttons[butt_back].getButtonH()/2)-0.01,
                                buttons[butt_back].getButtonZ()
                        );
                        buttons[butt_back].setButtonText("Back");
			
			// -- Setup send button.
                        buttons[butt_send].setWidthHeight(.17, 0.09);
                        buttons[butt_send].setCoords(
                                background.getBgX() + (buttons[butt_send].getButtonW()/2),
                                background.getBgY()+0.5,
                                0
                        );
                        buttons[butt_send].setButtonColor(.2, .2, .2);
                        buttons[butt_send].setButtonTextCoords(
                                (buttons[butt_send].getButtonX()+(buttons[butt_send].getButtonW()/2))-0.03,
                                buttons[butt_send].getButtonY()+(buttons[butt_send].getButtonH()/2)-0.01,
                                buttons[butt_send].getButtonZ()
                        );
                        buttons[butt_send].setButtonText("Send");

			// -- Setup request access button.
                        buttons[butt_requestAccess].setWidthHeight(.17, 0.09);
                        buttons[butt_requestAccess].setCoords(
                                buttons[butt_send].getButtonX(),
                                buttons[butt_send].getButtonY()+(buttons[butt_send].getButtonH()+0.03),
                                0
                        );
                        buttons[butt_requestAccess].setButtonColor(.2, .2, .2);
                        buttons[butt_requestAccess].setButtonTextCoords(
                                (buttons[butt_requestAccess].getButtonX()+(buttons[butt_requestAccess].getButtonW()/2))-0.03,
                                buttons[butt_requestAccess].getButtonY()+(buttons[butt_requestAccess].getButtonH()/2)-0.01,
                                buttons[butt_requestAccess].getButtonZ()
                        );
                        buttons[butt_requestAccess].setButtonText("Access");

			// -- Setup unread button.
                        buttons[butt_unread].setWidthHeight(.17, 0.09);
                        buttons[butt_unread].setCoords(
                                buttons[butt_send].getButtonX(),
                                buttons[butt_send].getButtonY()-(buttons[butt_send].getButtonH()+0.03),
                                0
                        );
                        buttons[butt_unread].setButtonColor(.2, .2, .2);
                        buttons[butt_unread].setButtonTextCoords(
                                (buttons[butt_unread].getButtonX()+(buttons[butt_unread].getButtonW()/2))-0.03,
                                buttons[butt_unread].getButtonY()+(buttons[butt_unread].getButtonH()/2)-0.01,
                                buttons[butt_unread].getButtonZ()
                        );
                        buttons[butt_unread].setButtonText("Unread");

			// -- Setup unread button.
                        buttons[butt_read].setWidthHeight(.17, 0.09);
                        buttons[butt_read].setCoords(
                                buttons[butt_unread].getButtonX(),
                                buttons[butt_unread].getButtonY()-(buttons[butt_unread].getButtonH()+0.03),
                                0
                        );
                        buttons[butt_read].setButtonColor(.2, .2, .2);
                        buttons[butt_read].setButtonTextCoords(
                                (buttons[butt_read].getButtonX()+(buttons[butt_read].getButtonW()/2))-0.03,
                                buttons[butt_read].getButtonY()+(buttons[butt_read].getButtonH()/2)-0.01,
                                buttons[butt_read].getButtonZ()
                        );
                        buttons[butt_read].setButtonText("Read");
                        Form::setButtons(buttons, buttonCount);

			// -- Setup text input
			textInputs[0].setDirSafeInput(false);
			textInputs[0].setLabel("");
			textInputs[0].setShowButtonText(false);
			textInputs[0].setWidthHeight(.5, 0.3);
			textInputs[0].setCoords(
				buttons[butt_send].getButtonX()+buttons[butt_send].getButtonW() + 0.04,
				buttons[butt_requestAccess].getButtonY() - textInputs[0].getH(),
				0		
			);
			textInputs[0].setInputCoords(
				textInputs[0].getX() + 0.02,
				textInputs[0].getY()+textInputs[0].getH() - 0.025,
				textInputs[0].getZ()
			);
			textInputs[0].setInputColor(1, 1, 1);
			textInputs[0].setEnterToDeselect(false);

			Form::setTextInputs(textInputs, 1);

			// -- Setup text 
			texts[0].setCoords(
				buttons[butt_requestAccess].getButtonX()+0.3,
				buttons[butt_requestAccess].getButtonY()+0.03,
				0			
			);
			Form::setTexts(texts, 1);
		}

		string getMessage(void){
			return textInputs[0].getInputData();
		}

		void setFriendKey(friendkey_t fren){
			friendKey = fren;
			string val = "Chatting with ";
			val += friendKey.alias;
			texts[0].setText(val);
			Form::setTexts(texts, 1);
		}

		friendkey_t getFriendKey(void){
			return friendKey;
		}

		void resetInput(){
			textInputs[0].setInputData("");
		}
};
