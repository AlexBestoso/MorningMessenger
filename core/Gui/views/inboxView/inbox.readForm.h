class InboxViewReadForm : public Form{
	private:
		MorningMessage morningMessage;
		FormBackground background;

		size_t buttonCount = 3;
		int butt_back = 0;
		int butt_save = 1;
		int butt_delete = 2;
		FormButton buttons[3];

		FormButton templateButton;
                FormSelectList selectList[1];
		mornmsg unreadMessages[200];
                size_t cnt = 0;

		FormScrollText scrollText[1];

		int keyId = -1;
		MorningSetup setup;

	public:
		InboxViewReadForm() : Form(){
			if(!setup.isSetup()){
                                return;
                        }
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

			// -- Setup save button.
                        buttons[butt_save].setWidthHeight(.17, 0.09);
                        buttons[butt_save].setCoords(
                                background.getBgX() + 0.04,
                                background.getBgY() + 0.03,
                                0
                        );
                        buttons[butt_save].setButtonColor(.2, .2, .2);
                        buttons[butt_save].setButtonTextCoords(
                                (buttons[butt_save].getButtonX()+(buttons[butt_save].getButtonW()/2))-0.03,
                                buttons[butt_save].getButtonY()+(buttons[butt_save].getButtonH()/2)-0.01,
                                buttons[butt_save].getButtonZ()
                        );
                        buttons[butt_save].setButtonText("Unread");

			// -- Setup delete button.
                        buttons[butt_delete].setWidthHeight(.17, 0.09);
                        buttons[butt_delete].setCoords(
				buttons[butt_save].getButtonX() + (buttons[butt_save].getButtonW() + 0.03),
				buttons[butt_save].getButtonY(),
                                0
                        );
                        buttons[butt_delete].setButtonColor(.2, .2, .2);
                        buttons[butt_delete].setButtonTextCoords(
                                (buttons[butt_delete].getButtonX()+(buttons[butt_delete].getButtonW()/2))-0.03,
                                buttons[butt_delete].getButtonY()+(buttons[butt_delete].getButtonH()/2)-0.01,
                                buttons[butt_delete].getButtonZ()
                        );
                        buttons[butt_delete].setButtonText("Delete");

			Form::setButtons(buttons, buttonCount);

			// -- Setup select list
                        selectList[0].setMenuWidthHeight(0.45, 0.6);
                        selectList[0].setMenuCoords(
                                background.getBgX(),
                                background.getBgY()+background.getBgW()-selectList[0].getMenuH(),
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

			// -- Setup Message Viewer
			scrollText[0].setMenuColor(.5, .5, .5);
                        scrollText[0].setMenuWidthHeight(background.getBgW()/2, 0.7);
                        scrollText[0].setMenuCoords(
                                background.getBgX()+background.getBgW() - scrollText[0].getMenuW(),
                                background.getBgY()+background.getBgH()-scrollText[0].getMenuH(),
                                0
                        );
                        scrollText[0].setTextFont(6);
                        scrollText[0].setEscapeText(false);
                        scrollText[0].setMaxCols(38);
                        scrollText[0].setMaxRows(15);
                        scrollText[0].setText("");
                        scrollText[0].setTextCoords(
                                scrollText[0].getMenuX()+0.03,
                                scrollText[0].getMenuY()+scrollText[0].getMenuH()-0.04,
                                0
                        );

                        Form::setScrollTexts(scrollText, 1);

		}

		void setFriendKeyId(int id){
			keyId = id;
		}

		void displayMessage(int id){
			if(id <0 || id >= cnt)
				return;
			string val = "[";
			val += unreadMessages[id].date + "] " +unreadMessages[id].subject + "\n\n";
			val += unreadMessages[id].message;
			scrollText[0].setText(val);
			Form::setScrollTexts(scrollText, 1);
		}

		void resetDisplay(void){
			scrollText[0].setText(" ");
                        Form::setScrollTexts(scrollText, 1);
		}

		void markAsUnread(int id){
			unreadMessages[id].read = false;
			morningMessage.updateMessage(unreadMessages[id]);
			updateListButtons();
			resetDisplay();
		}

		void deleteMessage(int id){
			morningMessage.deleteMessage(unreadMessages[id]);
			updateListButtons();
			resetDisplay();
		}

		void updateListButtons(void){
			cnt = morningMessage.getReadMessages(keyId, unreadMessages, 200);
                        if(cnt == 0){
				selectList[0].setSectionButtonOptions(NULL, 0);
                        	Form::setSelectLists(selectList, 1);
                                return;
			}
                        FormButton *buttons = new FormButton[cnt];

                        for(int i=0; i<cnt; i++){
                                string val = to_string(i+1);
                                val += ") ["+unreadMessages[i].date + "] ";
				val += unreadMessages[i].subject;
                                templateButton.setButtonText(val);
                                buttons[i] = templateButton;
                        }

                        selectList[0].setSectionButtonOptions(buttons, cnt);
                        Form::setSelectLists(selectList, 1);
                }
};
