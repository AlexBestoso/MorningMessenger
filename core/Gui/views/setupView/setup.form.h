class SetupViewForm : public Form{
	private:
		FormBackground background;
		
		const int sql_host = 0;
		const int sql_port = 1;
		const int sql_db = 2;
		const int sql_user = 3;
		const int sql_pass = 4;
		const int srv_host = 5;
		const int srv_port = 6;
		const int srv_user = 7;
		const int srv_pass = 8;
		size_t inputCount = 9;
		FormTextInput inputs[9];
		
		FormButton buttons[1];
	public:
		SetupViewForm() : Form(){
			// setup Background
			background.setUseTexture(true);
			background.setTexturePath("./assets/menu-setup.bmp");
			Form::setBackground(background);
			Form::setHideTitle(true);

		
			for(int i=0; i<inputCount; i++)
				inputs[i].setDirSafeInput(false);
			// setup sql host input
			inputs[sql_host].setCoords(
                 	       background.getBgX()+0.080,
                 	       background.getBgY()+.490,
                 	       background.getBgZ()
                	);
	                inputs[sql_host].setWidthHeight(.35, .075);
	
                	inputs[sql_host].setLabelCoords(
                	        inputs[sql_host].getX(),
                	        inputs[sql_host].getY()+.030,
                	        inputs[sql_host].getZ()
                	);
                	inputs[sql_host].setLabel("Sql Host :");
                	inputs[sql_host].setInputCoords(
                	        inputs[sql_host].getX()+0.06,
                	        inputs[sql_host].getY()+.030,
                	        inputs[sql_host].getZ()
                	);
			inputs[sql_host].setInput("127.0.0.1");

			// setup sql port input
			inputs[sql_port].setCoords(
				inputs[sql_host].getX(),
                                inputs[sql_host].getY()-0.08,
                                inputs[sql_host].getZ()
                        );
                        inputs[sql_port].setWidthHeight(.35, .075);

                        inputs[sql_port].setLabelCoords(
                                inputs[sql_port].getX(),
                                inputs[sql_port].getY()+.030,
                                inputs[sql_port].getZ()
                        );
                        inputs[sql_port].setLabel("Sql Port :");
                        inputs[sql_port].setInputCoords(
                                inputs[sql_port].getX()+0.06,
                                inputs[sql_port].getY()+.030,
                                inputs[sql_port].getZ()
                        );
                        inputs[sql_port].setInput("3306");

			// setup sql db input
                        inputs[sql_db].setCoords(
                                inputs[sql_port].getX(),
                                inputs[sql_port].getY()-0.08,
                                inputs[sql_port].getZ()
                        );
                        inputs[sql_db].setWidthHeight(.35, .075);

                        inputs[sql_db].setLabelCoords(
                                inputs[sql_db].getX(),
                                inputs[sql_db].getY()+.030,
                                inputs[sql_db].getZ()
                        );
                        inputs[sql_db].setLabel("Sql DB   :");
                        inputs[sql_db].setInputCoords(
                                inputs[sql_db].getX()+0.06,
                                inputs[sql_db].getY()+.030,
                                inputs[sql_db].getZ()
                        );
                        inputs[sql_db].setInput("m_msg");

			// setup sql user input
                        inputs[sql_user].setCoords(
                                inputs[sql_db].getX(),
                                inputs[sql_db].getY()-0.08,
                                inputs[sql_db].getZ()
                        );
                        inputs[sql_user].setWidthHeight(.35, .075);

                        inputs[sql_user].setLabelCoords(
                                inputs[sql_user].getX(),
                                inputs[sql_user].getY()+.030,
                                inputs[sql_user].getZ()
                        );
                        inputs[sql_user].setLabel("Sql User :");
                        inputs[sql_user].setInputCoords(
                                inputs[sql_user].getX()+0.06,
                                inputs[sql_user].getY()+.030,
                                inputs[sql_user].getZ()
                        );
                        inputs[sql_user].setInput("");

			// setup sql pass input
                        inputs[sql_pass].setCoords(
                                inputs[sql_user].getX(),
                                inputs[sql_user].getY()-0.08,
                                inputs[sql_user].getZ()
                        );
                        inputs[sql_pass].setWidthHeight(.35, .075);

                        inputs[sql_pass].setLabelCoords(
                                inputs[sql_pass].getX(),
                                inputs[sql_pass].getY()+.030,
                                inputs[sql_pass].getZ()
                        );
                        inputs[sql_pass].setLabel("Sql Pass :");
                        inputs[sql_pass].setInputCoords(
                                inputs[sql_pass].getX()+0.06,
                                inputs[sql_pass].getY()+.030,
                                inputs[sql_pass].getZ()
                        );
                        inputs[sql_pass].setInput("");	
			
			// setup server host input
                        inputs[srv_host].setCoords(
                               background.getBgX()+0.54,
                               background.getBgY()+.490,
                               background.getBgZ()
                        );
                        inputs[srv_host].setWidthHeight(.35, .075);

                        inputs[srv_host].setLabelCoords(
                                inputs[srv_host].getX(),
                                inputs[srv_host].getY()+.030,
                                inputs[srv_host].getZ()
                        );
                        inputs[srv_host].setLabel("Server Host :");
                        inputs[srv_host].setInputCoords(
                                inputs[srv_host].getX()+0.08,
                                inputs[srv_host].getY()+.030,
                                inputs[srv_host].getZ()
                        );
                        inputs[srv_host].setInput("127.0.0.1");

			// setup server port input
                        inputs[srv_port].setCoords(
				inputs[srv_host].getX(),
                                inputs[srv_host].getY()-0.08,
                                inputs[srv_host].getZ()
                        );
                        inputs[srv_port].setWidthHeight(.35, .075);

                        inputs[srv_port].setLabelCoords(
                                inputs[srv_port].getX(),
                                inputs[srv_port].getY()+.030,
                                inputs[srv_port].getZ()
                        );
                        inputs[srv_port].setLabel("Server Port :");
                        inputs[srv_port].setInputCoords(
                                inputs[srv_port].getX()+0.08,
                                inputs[srv_port].getY()+.030,
                                inputs[srv_port].getZ()
                        );
                        inputs[srv_port].setInput("21345");

			// setup server user input
                        inputs[srv_user].setCoords(
                                inputs[srv_port].getX(),
                                inputs[srv_port].getY()-0.08,
                                inputs[srv_port].getZ()
                        );
                        inputs[srv_user].setWidthHeight(.35, .075);

                        inputs[srv_user].setLabelCoords(
                                inputs[srv_user].getX(),
                                inputs[srv_user].getY()+.030,
                                inputs[srv_user].getZ()
                        );
                        inputs[srv_user].setLabel("Server User :");
                        inputs[srv_user].setInputCoords(
                                inputs[srv_user].getX()+0.08,
                                inputs[srv_user].getY()+.030,
                                inputs[srv_user].getZ()
                        );
                        inputs[srv_user].setInput("admin");

			// setup server pass input
                        inputs[srv_pass].setCoords(
                                inputs[srv_user].getX(),
                                inputs[srv_user].getY()-0.08,
                                inputs[srv_user].getZ()
                        );
                        inputs[srv_pass].setWidthHeight(.35, .075);

                        inputs[srv_pass].setLabelCoords(
                                inputs[srv_pass].getX(),
                                inputs[srv_pass].getY()+.030,
                                inputs[srv_pass].getZ()
                        );
                        inputs[srv_pass].setLabel("Server Pass :");
                        inputs[srv_pass].setInputCoords(
                                inputs[srv_pass].getX()+0.08,
                                inputs[srv_pass].getY()+.030,
                                inputs[srv_pass].getZ()
                        );
                        inputs[srv_pass].setInput("");

			Form::setTextInputs(inputs, 9);

			// Setup submit button
			buttons[0].setCoords(
                	        background.getBgX()+.390,
                	        background.getBgY()+0,
                	        background.getBgZ()+.03
                	);
                	buttons[0].setWidthHeight(.17, 0.09);
                	buttons[0].setButtonColor(.2, .2, .2);
                	buttons[0].setButtonTextCoords(
                	        (buttons[0].getButtonX()+(buttons[0].getButtonW()/2))-0.03,
                	        buttons[0].getButtonY()+(buttons[0].getButtonH()/2)-0.01,
                	        buttons[0].getButtonZ()
                	);
                	buttons[0].setButtonText("Done");
			Form::setButtons(buttons, 1);
		}

		string getSqlHostInput(void){
			return inputs[sql_host].getInputData();
		}

		unsigned int getSqlPortInput(void){
                        return (unsigned int)atoi(inputs[sql_port].getInputData().c_str());
                }

		string getSqlDatabaseInput(void){
                        return inputs[sql_db].getInputData();
                }

		string getSqlUserInput(void){
                        return inputs[sql_user].getInputData();
                }

		string getSqlPasswordInput(void){
                        return inputs[sql_pass].getInputData();
                }

		string getServerHostInput(void){
                        return inputs[srv_host].getInputData();
                }

		unsigned int getServerPortInput(void){
                        return (unsigned int)atoi(inputs[srv_port].getInputData().c_str());
                }

		string getServerUserInput(void){
                        return inputs[srv_user].getInputData();
                }

		string getServerPasswordInput(void){
                        return inputs[srv_pass].getInputData();
                }
};
