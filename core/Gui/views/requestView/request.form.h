class RequestViewForm : public Form{
	private:
		FormBackground background;

		size_t inputCount = 2;
		int input_host = 0;
		int input_port = 1;
		FormTextInput inputs[2];

		int buttonCount = 2;
		int butt_back = 0;
		int butt_submit = 1;
		FormButton buttons[2];

		 MorningSetup setup;
	public:
		RequestViewForm() : Form(){
			if(!setup.isSetup()){
                                return;
                        }
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-findfrenz.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			for(int i=0; i<inputCount; i++){
				inputs[i].setDirSafeInput(false);
			}

			// -- host input
			inputs[input_host].setWidthHeight(.32, .075);
			inputs[input_host].setCoords(
				background.getBgX()+background.getBgW()-inputs[input_host].getW()-0.3,
				background.getBgY()+0.50,
				0
			);
			inputs[input_host].setLabelCoords(
				inputs[input_host].getX()+.0006,
				inputs[input_host].getY()+0.030,
				inputs[input_host].getZ()	
			);
			inputs[input_host].setLabel("Host: ");
			inputs[input_host].setInputCoords(
				inputs[input_host].getX()+0.07,
				inputs[input_host].getY()+0.030,
				0	
			);
			inputs[input_host].setInput("");

			// -- port input
                        inputs[input_port].setWidthHeight(.32, .075);
                        inputs[input_port].setCoords(
                                background.getBgX()+background.getBgW()-inputs[input_port].getW()-0.3,
                                background.getBgY()+0.50-inputs[input_host].getH()-0.03,
                                0
                        );
                        inputs[input_port].setLabelCoords(
                                inputs[input_port].getX()+.0006,
                                inputs[input_port].getY()+0.030,
                                inputs[input_port].getZ()
                        );
                        inputs[input_port].setLabel("Port: ");
                        inputs[input_port].setInputCoords(
                                inputs[input_port].getX()+0.07,
                                inputs[input_port].getY()+0.030,
                                0
                        );
                        inputs[input_port].setInput("21345");

			// -- submiot button
			buttons[butt_submit].setWidthHeight(.11, .075);
			buttons[butt_submit].setButtonColor(.2, .2, .2);
			buttons[butt_submit].setCoords(
				background.getBgX()+background.getBgW()-buttons[butt_submit].getButtonW()-0.3,
                                background.getBgY()+0.50-(inputs[input_host].getH()*2)-0.03,
                                0
			);
			buttons[butt_submit].setButtonTextCoords(
                                (buttons[butt_submit].getButtonX()+(buttons[butt_submit].getButtonW()/2))-0.03,
                                buttons[butt_submit].getButtonY()+(buttons[butt_submit].getButtonH()/2)-0.01,
                                buttons[butt_submit].getButtonZ()
                        );
			buttons[butt_submit].setButtonText("Connect");

			// -- back button
			buttons[butt_back].setWidthHeight(.11, .075);
                        buttons[butt_back].setButtonColor(.2, .2, .2);
                        buttons[butt_back].setCoords(
                                background.getBgX()+background.getBgW()-inputs[input_port].getW()-0.3,
                                background.getBgY()+0.50-(inputs[input_host].getH()*2)-0.03,
                                0
                        );
                        buttons[butt_back].setButtonTextCoords(
                                (buttons[butt_back].getButtonX()+(buttons[butt_back].getButtonW()/2))-0.03,
                                buttons[butt_back].getButtonY()+(buttons[butt_back].getButtonH()/2)-0.01,
                                buttons[butt_back].getButtonZ()
                        );
                        buttons[butt_back].setButtonText("Cancel");

			Form::setButtons(buttons, buttonCount);
			Form::setTextInputs(inputs, inputCount);

		}

		string getHostName(void){
			return inputs[input_host].getInputData();
		}

		string getPort(void){
			return inputs[input_port].getInputData();
		}
};
