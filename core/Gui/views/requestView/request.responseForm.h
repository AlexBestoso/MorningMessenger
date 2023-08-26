class RequestViewResponseForm : public Form{
	private:
		MorningSetup setup;
		FormBackground background;
		
		size_t textCount = 1;
		int inquery = 0;
		FormText text[1];

		size_t inputCount = 1;
		int response = 0;
		FormTextInput inputs[2];

		int buttonCount = 1;
		int butt_submit = 0;
		FormButton buttons[1];
	public:
		RequestViewResponseForm() : Form(){
			if(!setup.isSetup()){
                                return;
                        }
			// -- setup background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-findfrenz-response.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			for(int i=0; i<inputCount; i++){
				inputs[i].setDirSafeInput(false);
			}

			// -- response input
			inputs[response].setWidthHeight(.32, .075);
			inputs[response].setCoords(
				background.getBgX()+0.12,
				background.getBgY()+0.05,
				0
			);
			inputs[response].setLabelCoords(
				inputs[response].getX()+.0006,
				inputs[response].getY()+0.030,
				inputs[response].getZ()	
			);
			inputs[response].setLabel("Response: ");
			inputs[response].setInputCoords(
				inputs[response].getX()+0.07,
				inputs[response].getY()+0.030,
				0	
			);
			inputs[response].setInput("");

			// -- submit button
			buttons[butt_submit].setWidthHeight(.11, .075);
			buttons[butt_submit].setButtonColor(.2, .2, .2);
			buttons[butt_submit].setCoords(
				background.getBgX()+background.getBgW()-buttons[butt_submit].getButtonW()-0.05,
                                inputs[response].getY(),
                                0
			);
			buttons[butt_submit].setButtonTextCoords(
                                (buttons[butt_submit].getButtonX()+(buttons[butt_submit].getButtonW()/2))-0.03,
                                buttons[butt_submit].getButtonY()+(buttons[butt_submit].getButtonH()/2)-0.01,
                                buttons[butt_submit].getButtonZ()
                        );
			buttons[butt_submit].setButtonText("Send");

			// -- network response text
			text[inquery].setCoords(
				inputs[response].getX(),
				background.getBgY()+0.4,
				0
			);


			Form::setTexts(text, textCount);
			Form::setButtons(buttons, buttonCount);
                        Form::setTextInputs(inputs, inputCount);

		}

		void setTextValue(string val){
			text[inquery].setText(val);
			Form::setTexts(text, textCount);
		}

		string getUserInput(void){
			return inputs[response].getInputData();
		}
		void resetUserInput(void){
			inputs[response].setInputData("");
			Form::setTextInputs(inputs, inputCount);
		}
};
