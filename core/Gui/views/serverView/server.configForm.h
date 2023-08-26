
class ServerConfigForm : public Form{
	private:
		FormBackground background;
		
		int input_servName = 0;
		int input_hostname = 1;
		int input_port = 2;
		size_t inputCount = 3;
		FormTextInput inputs[3];

		int butt_servName = 0;
		int butt_hostname = 1;
		int butt_port = 2;
		int butt_newkeys = 3;
		int butt_tormode = 4;
		int butt_back = 5;
		size_t buttonCount = 6;
                FormButton buttons[6];

		serverconfig_t cfg;
		MorningServer server;
		MorningSetup setup;

	public:
		ServerConfigForm() : Form(){
			if(!setup.isSetup()){
				return;
			}
			// Setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-server-config.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			// -- prep inputs
			for(int i=0; i<inputCount; i++)
                                inputs[i].setDirSafeInput(false);
			server.loadConfigs();
			cfg = server.getServerConfig();

			// -- Server name input
                        inputs[input_servName].setCoords(
                               background.getBgX()+0.080,
                               background.getBgY()+.540,
                               background.getBgZ()
                        );
                        inputs[input_servName].setWidthHeight(.32, .075);

                        inputs[input_servName].setLabelCoords(
                                inputs[input_servName].getX(),
                                inputs[input_servName].getY()+.030,
                                inputs[input_servName].getZ()
                        );
                        inputs[input_servName].setLabel("Nickname: ");
                        inputs[input_servName].setInputCoords(
                                inputs[input_servName].getX()+0.07,
                                inputs[input_servName].getY()+.030,
                                inputs[input_servName].getZ()
                        );

                        inputs[input_servName].setInput(cfg.serverName);

                        buttons[butt_servName].setWidthHeight(.11, 0.09);
                        buttons[butt_servName].setCoords(
                                inputs[input_servName].getX()+inputs[input_servName].getW(),
				inputs[input_servName].getY(),
                                background.getBgZ()
                        );
                        buttons[butt_servName].setButtonColor(.2, .2, .2);
                        buttons[butt_servName].setButtonTextCoords(
                                (buttons[butt_servName].getButtonX()+(buttons[butt_servName].getButtonW()/2))-0.03,
                                buttons[butt_servName].getButtonY()+(buttons[butt_servName].getButtonH()/2)-0.01,
                                buttons[butt_servName].getButtonZ()
                        );
                        buttons[butt_servName].setButtonText("Update");

			// -- Hostname input
                        inputs[input_hostname].setCoords(
                               background.getBgX()+0.080,
                               background.getBgY()+.340,
                               background.getBgZ()
                        );
                        inputs[input_hostname].setWidthHeight(.32, .075);

                        inputs[input_hostname].setLabelCoords(
                                inputs[input_hostname].getX(),
                                inputs[input_hostname].getY()+.030,
                                inputs[input_hostname].getZ()
                        );
                        inputs[input_hostname].setLabel("Hostname: ");
                        inputs[input_hostname].setInputCoords(
                                inputs[input_hostname].getX()+0.07,
                                inputs[input_hostname].getY()+.030,
                                inputs[input_hostname].getZ()
                        );

                        inputs[input_hostname].setInput(cfg.serverHost);

                        buttons[butt_hostname].setWidthHeight(.11, 0.09);
                        buttons[butt_hostname].setCoords(
                                inputs[input_hostname].getX()+inputs[input_hostname].getW(),
                                inputs[input_hostname].getY(),
                                background.getBgZ()
                        );
                        buttons[butt_hostname].setButtonColor(.2, .2, .2);
                        buttons[butt_hostname].setButtonTextCoords(
                                (buttons[butt_hostname].getButtonX()+(buttons[butt_hostname].getButtonW()/2))-0.03,
                                buttons[butt_hostname].getButtonY()+(buttons[butt_hostname].getButtonH()/2)-0.01,
                                buttons[butt_hostname].getButtonZ()
                        );
                        buttons[butt_hostname].setButtonText("Update");

			// -- port number input
                        inputs[input_port].setCoords(
                               background.getBgX()+0.080,
                               background.getBgY()+.140,
                               background.getBgZ()
                        );
                        inputs[input_port].setWidthHeight(.32, .075);

                        inputs[input_port].setLabelCoords(
                                inputs[input_port].getX(),
                                inputs[input_port].getY()+.030,
                                inputs[input_port].getZ()
                        );
                        inputs[input_port].setLabel("Port: ");
                        inputs[input_port].setInputCoords(
                                inputs[input_port].getX()+0.07,
                                inputs[input_port].getY()+.030,
                                inputs[input_port].getZ()
                        );

                        inputs[input_port].setInput(to_string(cfg.serverPort));

                        buttons[butt_port].setWidthHeight(.11, 0.09);
                        buttons[butt_port].setCoords(
                                inputs[input_port].getX()+inputs[input_port].getW(),
                                inputs[input_port].getY(),
                                background.getBgZ()
                        );
                        buttons[butt_port].setButtonColor(.2, .2, .2);
                        buttons[butt_port].setButtonTextCoords(
                                (buttons[butt_port].getButtonX()+(buttons[butt_port].getButtonW()/2))-0.03,
                                buttons[butt_port].getButtonY()+(buttons[butt_port].getButtonH()/2)-0.01,
                                buttons[butt_port].getButtonZ()
                        );
                        buttons[butt_port].setButtonText("Update");


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

			// -- new keypair button
                        buttons[butt_newkeys].setWidthHeight(.11, 0.09);
                        buttons[butt_newkeys].setCoords(
				background.getBgX() + background.getBgW() - buttons[butt_newkeys].getButtonW() -.005,
                               	background.getBgY()+.540,
                               	background.getBgZ()
                        );
                        buttons[butt_newkeys].setButtonColor(.2, .2, .2);
                        buttons[butt_newkeys].setButtonTextCoords(
                                (buttons[butt_newkeys].getButtonX()+(buttons[butt_newkeys].getButtonW()/2))-0.03,
                                buttons[butt_newkeys].getButtonY()+(buttons[butt_newkeys].getButtonH()/2)-0.01,
                                buttons[butt_newkeys].getButtonZ()
                        );
                        buttons[butt_newkeys].setButtonText("Genkeys");

			// -- toggle tor button
                        buttons[butt_tormode].setWidthHeight(.11, 0.09);
                        buttons[butt_tormode].setCoords(
                                background.getBgX() + background.getBgW() - buttons[butt_tormode].getButtonW() -.005,
                                background.getBgY()+.340,
                                background.getBgZ()
                        );
                        buttons[butt_tormode].setButtonColor(.2, .2, .2);
                        buttons[butt_tormode].setButtonTextCoords(
                                (buttons[butt_tormode].getButtonX()+(buttons[butt_tormode].getButtonW()/2))-0.03,
                                buttons[butt_tormode].getButtonY()+(buttons[butt_tormode].getButtonH()/2)-0.01,
                                buttons[butt_tormode].getButtonZ()
                        );
                        buttons[butt_tormode].setButtonText("tormode");


			Form::setTextInputs(inputs, inputCount);
                        Form::setButtons(buttons, buttonCount);

		}

		string serverNameInput(){
			return inputs[input_servName].getInputData();
		}
		string serverHostInput(){
			return inputs[input_hostname].getInputData();
		}
		string serverPort(){
			return inputs[input_port].getInputData();
		}
};
