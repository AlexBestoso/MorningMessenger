class MainViewForm : public Form{
	private:
		FormBackground background;

		const int srv_ctrl = __MAIN_VIEW__SERVERCTRL;
		const int find_friend = __MAIN_VIEW__FINDFRIENDS;
		const int mang_keys = __MAIN_VIEW__MANAGEKEYS;
		const int inbox = __MAIN_VIEW__INBOX;
		const int log_out = __MAIN_VIEW__LOGOUT;
		FormButton buttons[5];
	public:
		MainViewForm() : Form(){
			// Setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-main.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// setup buttons
			// -- server control button
			buttons[srv_ctrl].setCoords(
                                background.getBgX()+0.05,
                                background.getBgY()+0.39,
                                background.getBgZ()+.03
                        );
                        buttons[srv_ctrl].setWidthHeight(.17, 0.09);
                        buttons[srv_ctrl].setButtonColor(.2, .2, .2);
                        buttons[srv_ctrl].setButtonTextCoords(
                                (buttons[srv_ctrl].getButtonX()+(buttons[srv_ctrl].getButtonW()/2))-0.03,
                                buttons[srv_ctrl].getButtonY()+(buttons[srv_ctrl].getButtonH()/2)-0.01,
                                buttons[srv_ctrl].getButtonZ()
                        );
                        buttons[srv_ctrl].setButtonText("Server");

			// -- find friend button
			buttons[find_friend].setCoords(
				buttons[srv_ctrl].getX() + buttons[srv_ctrl].getButtonW()+0.1,
				buttons[srv_ctrl].getY(),
				buttons[srv_ctrl].getZ()
                        );
                        buttons[find_friend].setWidthHeight(.17, 0.09);
                        buttons[find_friend].setButtonColor(.2, .2, .2);
                        buttons[find_friend].setButtonTextCoords(
                                (buttons[find_friend].getButtonX()+(buttons[find_friend].getButtonW()/2))-0.03,
                                buttons[find_friend].getButtonY()+(buttons[find_friend].getButtonH()/2)-0.01,
                                buttons[find_friend].getButtonZ()
                        );
                        buttons[find_friend].setButtonText("Friends");

			// -- manage keys
			buttons[mang_keys].setCoords(
                                buttons[srv_ctrl].getX(),
                                buttons[srv_ctrl].getY() - 0.25,
                                buttons[srv_ctrl].getZ()
                        );
                        buttons[mang_keys].setWidthHeight(.17, 0.09);
                        buttons[mang_keys].setButtonColor(.2, .2, .2);
                        buttons[mang_keys].setButtonTextCoords(
                                (buttons[mang_keys].getButtonX()+(buttons[mang_keys].getButtonW()/2))-0.03,
                                buttons[mang_keys].getButtonY()+(buttons[mang_keys].getButtonH()/2)-0.01,
                                buttons[mang_keys].getButtonZ()
                        );
                        buttons[mang_keys].setButtonText("Keys");

			// -- Inbox
			buttons[inbox].setCoords(
                                buttons[mang_keys].getX() + buttons[srv_ctrl].getButtonW()+0.1,
                                buttons[mang_keys].getY(),
                                buttons[mang_keys].getZ()
                        );
                        buttons[inbox].setWidthHeight(.17, 0.09);
                        buttons[inbox].setButtonColor(.2, .2, .2);
                        buttons[inbox].setButtonTextCoords(
                                (buttons[inbox].getButtonX()+(buttons[inbox].getButtonW()/2))-0.03,
                                buttons[inbox].getButtonY()+(buttons[inbox].getButtonH()/2)-0.01,
                                buttons[inbox].getButtonZ()
                        );
                        buttons[inbox].setButtonText("Inbox");

			// -- logout
			buttons[log_out].setCoords(
                                buttons[inbox].getX()+0.45,
                                buttons[inbox].getY()-0.06,
                                buttons[inbox].getZ()
                        );
                        buttons[log_out].setWidthHeight(.17, 0.09);
                        buttons[log_out].setButtonColor(.2, .2, .2);
                        buttons[log_out].setButtonTextCoords(
                                (buttons[log_out].getButtonX()+(buttons[log_out].getButtonW()/2))-0.03,
                                buttons[log_out].getButtonY()+(buttons[log_out].getButtonH()/2)-0.01,
                                buttons[log_out].getButtonZ()
                        );
                        buttons[log_out].setButtonText("Logout");

                        Form::setButtons(buttons, 5);
		}
};
