class LoginViewForm : public Form{
	private:
		FormBackground background;
		const int user_login = 0;
		const int user_password = 1;
		size_t inputCount = 2;
		FormTextInput inputs[2];
		FormButton buttons[1];
		FormText texts[1];

		bool accessabilityMode = false;
		size_t elementCount = 3;
		unsigned int currentElement = 0;

		void accessabilityFocus(void){
			if(currentElement >= elementCount)
				currentElement = elementCount-1;
			else if(currentElement < 0)
				currentElement = 0;
			switch(currentElement){
				case 0:
					inputs[user_password].setFocus(false);	
					inputs[user_login].setFocus(true);
					Form::setTextInputs(inputs, inputCount);
					buttons[0].setHover(false);
					Form::setButtons(buttons, 1);
					break;
				case 1:
					inputs[user_password].setFocus(true);
                                        inputs[user_login].setFocus(false);
                                        Form::setTextInputs(inputs, inputCount);
					Form::setButtons(buttons, 1);
                                        Form::setTextInputs(inputs, inputCount);
					buttons[0].setHover(false);
					Form::setButtons(buttons, 1);
					break;
				case 2:
					inputs[user_password].setFocus(false);
                                        inputs[user_login].setFocus(false);
                                        Form::setTextInputs(inputs, inputCount);
					buttons[0].setHover(true);
					Form::setButtons(buttons, 1);
					break;
			}
		}
		
	public:
		LoginViewForm() : Form(){
			// configure background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-login.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

			
			// setup inputs
			for(int i=0; i<inputCount; i++)
				inputs[i].setDirSafeInput(false);
			// setup username input
			inputs[user_login].setCoords(
				background.getBgX()+0.25,
				background.getBgY()+0.5,
				background.getBgZ()		
			);
			inputs[user_login].setWidthHeight(.36, .075);
			inputs[user_login].setLabelCoords(
				inputs[user_login].getX(),
				inputs[user_login].getY()+0.030,
				inputs[user_login].getZ()
			);
			inputs[user_login].setLabel("User :");
			inputs[user_login].setInputCoords(
				inputs[user_login].getX()+0.06,
				inputs[user_login].getY()+0.030,
				inputs[user_login].getZ()
			);
			inputs[user_login].setInput("");
			
			// Setup password input
			inputs[user_password].setCoords(
				inputs[user_login].getX(),
				inputs[user_login].getY()-0.2,
				inputs[user_login].getZ()
                        );
                        inputs[user_password].setWidthHeight(.36, .075);
                        inputs[user_password].setLabelCoords(
                                inputs[user_password].getX(),
                                inputs[user_password].getY()+0.030,
                                inputs[user_password].getZ()
                        );
                        inputs[user_password].setLabel("password :");
                        inputs[user_password].setInputCoords(
                                inputs[user_password].getX()+0.07,
                                inputs[user_password].getY()+0.030,
                                inputs[user_password].getZ()
                        );
                        inputs[user_password].setInput("");

			Form::setTextInputs(inputs, inputCount);

			// Setup Submit button
			buttons[0].setCoords(
				background.getBgX()+.390,
                                background.getBgY()+0.03,
                                background.getBgZ()+.03
			);
			buttons[0].setWidthHeight(.17, 0.099);
			buttons[0].setButtonColor(.2, .2, .2);
			buttons[0].setButtonTextCoords(
				(buttons[0].getButtonX()+(buttons[0].getButtonW()/2))-0.03,
                                buttons[0].getButtonY()+(buttons[0].getButtonH()/2)-0.01,
                                buttons[0].getButtonZ()
			);
			buttons[0].setButtonText("Login");
			Form::setButtons(buttons, 1);

			// setup auth failed message.
			texts[0].setHideText(true);
			texts[0].setText("Invalid Credentials");
			texts[0].setTextColor(1, 0, 0);
			texts[0].setCoords(
				buttons[0].getX()+0.3,
				buttons[0].getY()+0.05,
				buttons[0].getZ()
			);
			texts[0].setFontById(3);
			Form::setTexts(texts, 1);
		}

		void resetInputs(){
			for(int i=0; i<inputCount; i++){
				inputs[i].setInputData("");
			}
		}

		void hideAuthError(bool value){
			texts[0].setHideText(value);
			Form::setTexts(texts, 1);
		}

		string getUsernameInput(void){
                        return inputs[user_login].getInputData();
                }
                string getPasswordInput(void){
                        return inputs[user_password].getInputData();
                }

		void accessabilityNext(void){
			currentElement = (currentElement + 1) % elementCount;
			accessabilityFocus();
		}
		void accessabilityPrevious(void){
			if(((int)currentElement-1) < 0)
				currentElement = elementCount-1;
			else
				currentElement -= 1;
			accessabilityFocus();
		}
		void setAccessability(bool val){
			accessabilityMode = val;
		}
		void setAccessabilityElement(int id){
			if(id >= elementCount)
				currentElement = elementCount-1;
			else if(id < 0)
				currentElement = 0;
			else
				currentElement = id;
			accessabilityFocus();
		}
		int getAccessabilityId(void){
			return currentElement;
		}
		bool getAccessability(void){
			return accessabilityMode;
		}
};
