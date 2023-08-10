class FormDropDown : public CoreObject{
	private:
	float menuX = 0;
	float menuY = 0;
	float menuZ = 0;
	float menuW = .3;
	float menuH = .3;
	float menuR = 0;
	float menuG = 0;
	float menuB = 0;


	FormButton sectionButton;

	FormButton *sectionButtonOptions = NULL;
	size_t sectionButtonOptionCount = 0;
	int maxOptionsVisible = 0;
	int optionsMin = 0;
	int optionsMax = 0;
	float **visibleButtonPositions = NULL;
	float optionsOffset = .3;
	int clickedOption = -1;

	bool showDropDown = false;
	bool hovered = false;

	void applyMenuOptionsPos(){
		int ctr = 0;
		for(int i=optionsMin; i<optionsMax; i++){
			if(i>=sectionButtonOptionCount)
				break;
			sectionButtonOptions[i].setCoords(
				visibleButtonPositions[ctr][0],
				visibleButtonPositions[ctr][1],
				visibleButtonPositions[ctr][2]
			);
			sectionButtonOptions[i].setButtonTextCoords(
				visibleButtonPositions[ctr][3],
				visibleButtonPositions[ctr][4],
				visibleButtonPositions[ctr][5]
			);
			ctr++;
		}
	}

	public:

	float getMenuX(){
		return menuX;
	}
	float getMenuY(){
		return menuY;
	}
	float getMenuZ(){
		return menuZ;
	}
	float getMenuW(){
		return menuW;
	}
	float getMenuH(){
		return menuH;
	}

	int getClickedOption(){
		return clickedOption;
	}

	void activelyHovered(float x, float y){
		y = y * -1;
		if( ( x >= menuX && x <= (menuX + menuW) )
                && ( y >= menuY && y <= (menuY + menuH))){
                        hovered = true;
                }else{
                        hovered = false;
                }
	}

	void setOptionsOffset(float offset){
		optionsOffset = offset;
	}
	void setMaxVisibleOptions(int count){
		maxOptionsVisible = count;
		optionsMax = count;
		optionsMin = 0;
		if(visibleButtonPositions != NULL){
			delete[] visibleButtonPositions;
			visibleButtonPositions = NULL;
		}
		if(count == 0)
			return;

		visibleButtonPositions = new float*[maxOptionsVisible];
		for(int i=0; i<maxOptionsVisible; i++){
			visibleButtonPositions[i] = new float[6]; // x, y, z | text x, text y, text z
		}
	}

	void decrementVisibleOptions(){
		if(optionsMax < sectionButtonOptionCount){
			optionsMax++;
			optionsMin++;
			applyMenuOptionsPos();
		}
	}
	void incrementVisibleOptions(){
		if(optionsMin > 0){
			optionsMin--;
			optionsMax--;
			applyMenuOptionsPos();
		}
	}
	
	void setSectionButton(FormButton button){
		sectionButton = button;
	}

	void setSectionButtonOptions(FormButton *buttons, size_t count){
		sectionButtonOptions = buttons;
		sectionButtonOptionCount = count;
		
		if(count > 0 && visibleButtonPositions != NULL && maxOptionsVisible > 0){
			float startX = buttons[0].getButtonX();
			float startY = buttons[0].getButtonY();
			float startZ = buttons[0].getButtonZ();
			float startW = buttons[0].getButtonW();
			float startH = buttons[0].getButtonH();
			float startTextX = buttons[0].getTextX();
			float startTextY = buttons[0].getTextY();
			float startTextZ = buttons[0].getTextZ();
			
			for(int i=0; i<maxOptionsVisible; i++){
				visibleButtonPositions[i][0] = startX;
				visibleButtonPositions[i][1] = startY - (startH*i) - (optionsOffset*i);
				visibleButtonPositions[i][2] = startZ;
				visibleButtonPositions[i][3] = startTextX;
				visibleButtonPositions[i][4] = startTextY - (startH*i) - (optionsOffset*i);
				visibleButtonPositions[i][5] = startTextZ;

			}
			applyMenuOptionsPos();
		}
	}

	void setMenuCoords(float x, float y, float z){
		menuX = x;
		menuY = y; 
		menuZ = z;
	}
	
	void setMenuWidthHeight(float w, float h){
		menuW = w;
		menuH = h;
	}

	void setMenuColor(float r, float g, float b){
		menuR = r;
		menuG = g;
		menuB = b;
	}

	void run(void){
		sectionButton.run();
		if(showDropDown){
			CoreObject::setColor(menuR, menuG, menuB);
			CoreObject::drawRectangle(menuX, menuY, menuZ, menuW, menuH);
			if(maxOptionsVisible <= 0){
				for(int i=0; i<sectionButtonOptionCount; i++){
					sectionButtonOptions[i].run();
				}
			}else{
				for(int i=optionsMin; i<optionsMax; i++){
					if(i < 0 || i >= sectionButtonOptionCount){
						break; // prevent buffer overflow;
					}
					sectionButtonOptions[i].run();
				}
			}

		}
	}

	int mousePassive(float x, float y){
		sectionButton.mousePassive(x, y);
		if(showDropDown){
			activelyHovered(x, y);
			if(maxOptionsVisible <= 0){
                                for(int i=0; i<sectionButtonOptionCount; i++){
                                        sectionButtonOptions[i].mousePassive(x, y);
                                }
                        }else{
                                for(int i=optionsMin; i<optionsMax; i++){
                                        if(i < 0 || i >= sectionButtonOptionCount){
                                                break; // prevent buffer overflow;
                                        }
                                        sectionButtonOptions[i].mousePassive(x, y);
                                }
                        }
			
		}
		return -1;
	}
	
	int mouseClick(int button, int state, float x, float y){
		int val = sectionButton.mouseClick(button, state, x, y);
		if(showDropDown == false){
			if(val == 1){
				showDropDown = true;
			}
		}else if(showDropDown == true){
			if(hovered){
				if(button == 3 && state == 1){
					incrementVisibleOptions();
				}else if(button == 4 && state == 1){
					decrementVisibleOptions();
				}
				if(maxOptionsVisible <= 0){
                        	        for(int i=0; i<sectionButtonOptionCount; i++){
                        	                val = sectionButtonOptions[i].mouseClick(button, state, x, y);
						if(val != -1){
							clickedOption = i;
							return val;
						}
						clickedOption = -1;
                        	        }
                        	}else{
                        	        for(int i=optionsMin; i<optionsMax; i++){
                        	                if(i < 0 || i >= sectionButtonOptionCount){
                        	                        break; // prevent buffer overflow;
                        	                }
                        	                val = sectionButtonOptions[i].mouseClick(button, state, x, y);
						if(val != -1){
                                                        clickedOption = i;
                                                        return val;
                                                }
						clickedOption = -1;
                        	        }
                        	}
			}else{
				optionsMax = maxOptionsVisible;
                		optionsMin = 0;
				applyMenuOptionsPos();
				showDropDown = false;
			}
		}
		return -1;
	}
};
