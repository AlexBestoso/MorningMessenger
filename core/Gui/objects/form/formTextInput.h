//TODO: Finish converting this class into a text input.
class FormTextInput : public CoreObject{
	private:
	int returnValue = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float w = .25;
	float h = .25;
	
	float red = 0;
	float green = 0;
	float blue = 0;

	float redHover = .5;
	float greenHover = .5;
	float blueHover = .5;

	float redFocus = .25;
	float greenFocus = .25;
	float blueFocus = .25;
	
	int buttonContext = 0;

	bool hover = false;

	void *fontBitMap = GLUT_BITMAP_HELVETICA_18;
	string label = "input";
	float labelX = 0;
	float labelY = 0;
	float labelZ = 0;
	float redLabel = 1;
	float greenLabel = 1;
	float blueLabel = 1;
	bool showButtonText = true;
	bool focused = false;

	string inputData;
	float redInput = 1;
	float greenInput = 1;
	float blueInput = 1;
	float inputX = 0;
	float inputY = 0;
	float inputZ = 0;

	bool dirSafeInput = true;

	public:

	FormTextInput(){
	
	}

	void setDirSafeInput(bool val){
		dirSafeInput = val;
	}

	void reset(){
		inputData = "";
		focused = false;
		hover = false;
	}

	float getX(){
		return x;
	}
	float getY(){
		return y;
	}
	float getZ(){
		return z;
	}
	float getW(){
		return w;
	}
	float getH(){
		return h;
	}

	string getInputData(void){
		return inputData;
	}

	void setInputData(string data){
		inputData = data;
	}

	void setShowButtonText(bool val){
		showButtonText = val;
	}
	void setInput(string inputVal){
		inputData = inputVal;
	}
	void setLabel(string text){
		label = text;
	}
	void setButtonFont(void* font){
		fontBitMap = font;
	}
	void setLabelCoords(float x, float y, float z){
		labelX = x;
		labelY = y;
		labelZ = z;
	}
	void setLabelColor(float r, float g, float b){
		redLabel = r;
		greenLabel = g;
		blueLabel = b;
	}
	void setInputCoords(float x, float y, float z){
		inputX = x;
		inputY = y;
		inputZ = z;
	}
	void setInputColor(float r, float g, float b){
                redInput = r;
                greenInput = g;
                blueInput = b;
        }
	
	void setContext(int context){
		buttonContext = context;
	}
	int getContext(void){
		return buttonContext;
	}

	void setCoords(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void setWidthHeight(float w, float h){
		this->w = w;
		this->h = h;
	}

	void setButtonColor(float r, float g, float b){
		red = r;
		green = g;
		blue = b;
	}

	void setHoverColor(float r, float g, float b){
		redHover = r;
		greenHover = g;
		blueHover = b;
	}

	void setFocusColor(float r, float g, float b){
                redFocus = r;
                greenFocus = g;
                blueFocus = b;
        }

	void run(){
		/* Core Container.*/
		if(this->focused){
			CoreObject::setColor(redFocus, greenFocus, blueFocus);
		}else if(!hover){
			CoreObject::setColor(red, green, blue);
		}else{
			CoreObject::setColor(redHover, greenHover, blueHover);
		}
		CoreObject::drawRectangle(this->x, this->y, this->z, this->w, this->h);

		/* Label Text*/
		CoreObject::setColor(redLabel, greenLabel, blueLabel);
		CoreObject::drawText(
			this->labelX,
			this->labelY,
			this->labelZ,
			this->label,
			GLUT_BITMAP_HELVETICA_12
		);

		/* Input Data Text */
		CoreObject::setColor(redInput, greenInput, blueInput);
		CoreObject::drawText(
			this->inputX,
			this->inputY,
			this->inputZ,
			this->inputData,
			GLUT_BITMAP_HELVETICA_12
		);
	}
	
	void mousePassive(float x, float y){
		y = y * -1;
		if( ( x >= this->x && x <= (this->x+this->w) ) 
		&& ( y >= this->y && y <= (this->y+this->h))){
			hover = true;
		}else{
			hover = false;
		}
	}
	
	/*
	 * Encodes the clicked button and state if clicked while hovering
	 *	bits  1000 0
	 * 	button -^  ^--state 
	 * 	to get button, (value >> 1) & 0b1111
	 * 	to get state, value & 1
	 * */	
	int mouseClick(int button, int state, float x, float y){
		if(hover){
			if(button == 0 && state == 1){
				this->focused = true;
			}
		}else{
			if(button == 0 && state == 1){
				this->focused = false;
			}
		}
		return -1;
	}

	int keyDown(unsigned char key, int mouseX, int mouseY){
		if(this->focused){
			if(key == 0x08){
				if(this->inputData.size() > 0)
					this->inputData.pop_back();
			}else if(key == 0x0d){
                                this->focused = false;
                        }else if(dirSafeInput){
				if((key >= 0x61 && key <= 0x7a) || (key >= 0x41 && key <= 0x5a) || (key >= 0x30 && key <= 0x39) || key == 0x20){
					this->inputData += key;
				}
			}else if(!dirSafeInput){
				this->inputData += key;
			}
		}
		return 0;
	}
};
