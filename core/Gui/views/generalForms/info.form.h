class InfoForm : public Form{
	private:
		FormBackground background;
		FormText text[1];
		FormButton buttons[1];
	public:

		InfoForm() : Form(){
			// -- setup background
                        background.setUseTexture(true);
                        background.setTexturePath("./assets/general-info.bmp");
                        Form::setBackground(background);
                        Form::setHideTitle(true);
			// Message text
			text[0].setCoords(
                                background.getBgX()+0.16,
                                background.getBgY()+0.47,
                                0
                        );

			// close button
			buttons[0].setWidthHeight(.11, .075);
                        buttons[0].setButtonColor(.2, .2, .2);
                        buttons[0].setCoords(
                                background.getBgX()+background.getBgW()-buttons[0].getButtonW()-0.05,
                                background.getBgY()+(0.075/4),
                                0
                        );
                        buttons[0].setButtonTextCoords(
                                (buttons[0].getButtonX()+(buttons[0].getButtonW()/2))-0.03,
                                buttons[0].getButtonY()+(buttons[0].getButtonH()/2)-0.01,
                                buttons[0].getButtonZ()
                        );
                        buttons[0].setButtonText("Close");

                        Form::setTexts(text, 1);
                        Form::setButtons(buttons, 1);
		}

		void setTextValue(string val){
                        text[0].setText(val);
                        Form::setTexts(text, 1);
                }
};
