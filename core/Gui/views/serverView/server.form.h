class ServerViewForm : public Form{
	private:
		FormBackground background;
		int butt_config = 0;
		int butt_start = 1;
		int butt_stop = 2;
		int butt_restart = 3;
		int butt_status = 4;
		int butt_back = 5;
		FormButton buttons[6];

	public:
		ServerViewForm() : Form(){
			// Setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-server.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// setup buttons
			// -- config button
			buttons[butt_config].setCoords(
                                background.getBgX()+0.07,
                                background.getBgY()+0.55,
                                background.getBgZ()+.03
                        );
                        buttons[butt_config].setWidthHeight(.17, 0.09);
                        buttons[butt_config].setButtonColor(.2, .2, .2);
                        buttons[butt_config].setButtonTextCoords(
                                (buttons[butt_config].getButtonX()+(buttons[butt_config].getButtonW()/2))-0.03,
                                buttons[butt_config].getButtonY()+(buttons[butt_config].getButtonH()/2)-0.01,
                                buttons[butt_config].getButtonZ()
                        );
                        buttons[butt_config].setButtonText("Config");

			// -- start button
			buttons[butt_start].setCoords(
				buttons[butt_config].getX() + buttons[butt_config].getButtonW()+0.1,
				buttons[butt_config].getY(),
				buttons[butt_config].getZ()
                        );
                        buttons[butt_start].setWidthHeight(.17, 0.09);
                        buttons[butt_start].setButtonColor(.2, .2, .2);
                        buttons[butt_start].setButtonTextCoords(
                                (buttons[butt_start].getButtonX()+(buttons[butt_start].getButtonW()/2))-0.03,
                                buttons[butt_start].getButtonY()+(buttons[butt_start].getButtonH()/2)-0.01,
                                buttons[butt_start].getButtonZ()
                        );
                        buttons[butt_start].setButtonText("Start");

			// -- stop button
			buttons[butt_stop].setCoords(
                                buttons[butt_config].getX(),
                                buttons[butt_config].getY() - 0.25,
                                buttons[butt_config].getZ()
                        );
                        buttons[butt_stop].setWidthHeight(.17, 0.09);
                        buttons[butt_stop].setButtonColor(.2, .2, .2);
                        buttons[butt_stop].setButtonTextCoords(
                                (buttons[butt_stop].getButtonX()+(buttons[butt_stop].getButtonW()/2))-0.03,
                                buttons[butt_stop].getButtonY()+(buttons[butt_stop].getButtonH()/2)-0.01,
                                buttons[butt_stop].getButtonZ()
                        );
                        buttons[butt_stop].setButtonText("Stop");

			// -- restart button
			buttons[butt_restart].setCoords(
                                buttons[butt_stop].getX() + buttons[butt_stop].getButtonW()+0.1,
                                buttons[butt_stop].getY(),
                                buttons[butt_stop].getZ()
                        );
                        buttons[butt_restart].setWidthHeight(.17, 0.09);
                        buttons[butt_restart].setButtonColor(.2, .2, .2);
                        buttons[butt_restart].setButtonTextCoords(
                                (buttons[butt_restart].getButtonX()+(buttons[butt_restart].getButtonW()/2))-0.03,
                                buttons[butt_restart].getButtonY()+(buttons[butt_restart].getButtonH()/2)-0.01,
                                buttons[butt_restart].getButtonZ()
                        );
                        buttons[butt_restart].setButtonText("Restart");

			// -- status button
			buttons[butt_status].setCoords(
                                buttons[butt_stop].getX(),
                                buttons[butt_stop].getY()-0.25,
                                buttons[butt_stop].getZ()
                        );
                        buttons[butt_status].setWidthHeight(.17, 0.09);
                        buttons[butt_status].setButtonColor(.2, .2, .2);
                        buttons[butt_status].setButtonTextCoords(
                                (buttons[butt_status].getButtonX()+(buttons[butt_status].getButtonW()/2))-0.03,
                                buttons[butt_status].getButtonY()+(buttons[butt_status].getButtonH()/2)-0.01,
                                buttons[butt_status].getButtonZ()
                        );
                        buttons[butt_status].setButtonText("Status");

			// -- back button
			buttons[butt_back].setWidthHeight(.11, 0.09);
			buttons[butt_back].setCoords(
				background.getBgX(),
				background.getBgY()+background.getBgH() - buttons[butt_back].getButtonH() - 0.05,
				background.getBgZ()		
			);
			buttons[butt_back].setButtonColor(.2, .2, .2);
			buttons[butt_back].setButtonTextCoords(
				(buttons[butt_back].getButtonX()+(buttons[butt_back].getButtonW()/2))-0.03,
                                buttons[butt_back].getButtonY()+(buttons[butt_back].getButtonH()/2)-0.01,
                                buttons[butt_back].getButtonZ()	
			);
			buttons[butt_back].setButtonText("Back");
                        Form::setButtons(buttons, 6);
		}
};
