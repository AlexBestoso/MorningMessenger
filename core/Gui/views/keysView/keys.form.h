#define __KEYS_VIEW__BACK 0
#define __KEYS_VIEW__TRUSTED 1
#define __KEYS_VIEW__UNTRUSTED 2
class KeysForm : public Form{
	private:
		FormBackground background;

		int buttonCount = 3;
		int butt_back = 0;
		int butt_trusted = 1;
		int butt_untrusted = 2;
		FormButton buttons[3];
	public:
		KeysForm() : Form(){
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-keys.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// -- back button
			buttons[butt_back].setWidthHeight(.11, .075);
                        buttons[butt_back].setButtonColor(.2, .2, .2);
                        buttons[butt_back].setCoords(
                                background.getBgX(),
                                background.getBgY(),
                                0
                        );
                        buttons[butt_back].setButtonTextCoords(
                                (buttons[butt_back].getButtonX()+(buttons[butt_back].getButtonW()/2))-0.03,
                                buttons[butt_back].getButtonY()+(buttons[butt_back].getButtonH()/2)-0.01,
                                buttons[butt_back].getButtonZ()
                        );
                        buttons[butt_back].setButtonText("Back");

			// -- Trusted button
                        buttons[butt_trusted].setWidthHeight(.11, .075);
                        buttons[butt_trusted].setButtonColor(.2, .2, .2);
                        buttons[butt_trusted].setCoords(
                                background.getBgX()+0.05,
                                background.getBgY()+.3,
                                0
                        );
                        buttons[butt_trusted].setButtonTextCoords(
                                (buttons[butt_trusted].getButtonX()+(buttons[butt_trusted].getButtonW()/2))-0.03,
                                buttons[butt_trusted].getButtonY()+(buttons[butt_trusted].getButtonH()/2)-0.01,
                                buttons[butt_trusted].getButtonZ()
                        );
                        buttons[butt_trusted].setButtonText("Trusted");

			// -- Untrusted button
                        buttons[butt_untrusted].setWidthHeight(.11, .075);
                        buttons[butt_untrusted].setButtonColor(.2, .2, .2);
                        buttons[butt_untrusted].setCoords(
					buttons[butt_trusted].getX()+0.5,
					buttons[butt_trusted].getY(),
					0
                        );
                        buttons[butt_untrusted].setButtonTextCoords(
                                (buttons[butt_untrusted].getButtonX()+(buttons[butt_untrusted].getButtonW()/2))-0.03,
                                buttons[butt_untrusted].getButtonY()+(buttons[butt_untrusted].getButtonH()/2)-0.01,
                                buttons[butt_untrusted].getButtonZ()
                        );
                        buttons[butt_untrusted].setButtonText("Untrusted");

			Form::setButtons(buttons, buttonCount);

		}

};
