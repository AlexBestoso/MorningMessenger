class FormButton : public CoreObject{
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
	
	int buttonContext = 0;

	bool hover = false;

	void *fontBitMap = GLUT_BITMAP_HELVETICA_18;
	string buttonText = "button";
	float textX = 0;
	float textY = 0;
	float textZ = 0;
	float textRed = .5;
	float textGreen = .5;
	float textBlue = .5;
	bool showButtonText = true;

	public:

	FormButton(int returnValue){
	
	}

	FormButton(){
	
	}

	string getButtonText(){
		return buttonText;
	}

	float getButtonX(){
		return x;
	}
	float getButtonY(){
		return y;
	}
	float getButtonZ(){
		return z;
	}
	float getX(void){
		return x;
	}
	float getY(void){
		return y;
	}
	float getZ(void){
		return z;
	}
	float getButtonW(){
		return w;
	}
	float getButtonH(){
		return h;
	}
	float getTextX(){
		return textX;
	}
	float getTextY(){
		return textY;
	}
	float getTextZ(){
		return textZ;
	}

	void setShowButtonText(bool val){
		showButtonText = val;
	}
	void setButtonText(string text){
		buttonText = text;
	}
	void setButtonFont(void* font){
		fontBitMap = font;
	}
	void setButtonTextCoords(float x, float y, float z){
		textX = x;
		textY = y;
		textZ = z;
	}
	void setTextColor(float r, float g, float b){
		textRed = r;
		textGreen = g;
		textBlue = b;
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

	void run(){
		if(!hover){
			CoreObject::setColor(red, green, blue);
		}else{
			CoreObject::setColor(redHover, greenHover, blueHover);
		}
		CoreObject::drawRectangle(x, y, z, w, h);
		if(showButtonText){
			CoreObject::setColor(textRed, textGreen, textBlue);
			CoreObject::drawText(textX, textY, textZ, buttonText, fontBitMap);
		}
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
			int ret = 0;
			ret += (button << 1) + (state)<<0;
			return ret;
		}else{
			return -1;
		}
	}
};
