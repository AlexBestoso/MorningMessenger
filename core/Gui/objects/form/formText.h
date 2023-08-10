class FormText : public CoreObject{
	private:
	void* titleFont = GLUT_BITMAP_TIMES_ROMAN_24;
	string titleText = "Sample Text";

	float red = 0;
	float green = 0;
	float blue = 0;

	float x = 0;
	float y = 0;
	float z = 0;

	bool hideText = false;
	public:

	void setHideText(bool v){
		hideText = v;
	}
	bool getHideText(void){
		return hideText;
	}
	void setText(string text){
		titleText = text;
	}

	void setFont(void *font){
		titleFont = font;
	}

	void setCoords(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
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

	void setFontById(int id){
		switch(id){
			case 0:
				titleFont = GLUT_BITMAP_9_BY_15;
				break;
			case 1:
				titleFont = GLUT_BITMAP_8_BY_13;
				break;
			case 2:
				titleFont = GLUT_BITMAP_TIMES_ROMAN_10;
				break;
			case 3: 
				setFont(GLUT_BITMAP_TIMES_ROMAN_24);
				break;
			case 4:
				titleFont = GLUT_BITMAP_HELVETICA_10;
				break;
			case 5:
				titleFont = GLUT_BITMAP_HELVETICA_12;
				break;
			case 6:
				titleFont = GLUT_BITMAP_HELVETICA_18;
				break;
			default:
				titleFont = GLUT_BITMAP_TIMES_ROMAN_24;
				break;
		}
	}

	void setTextColor(float r, float g, float b){
		red = r;
		green = g;
		blue = b;
	}

	void run(){
		CoreObject::setColor(red, green, blue);
		CoreObject::drawText(x, y, z, titleText, titleFont);
	}
};
