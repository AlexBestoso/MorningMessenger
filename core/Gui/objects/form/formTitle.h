class FormTitle : public CoreObject{
	private:
	void* titleFont = GLUT_BITMAP_TIMES_ROMAN_24;
	string titleText = "Sample Text";

	float red = 0;
	float green = 0;
	float blue = 0;

	float fontHeightOffset = 0;
	float fontWidthOffset = 0;

	float boundsX = 0;
	float boundsY = 0;
	float boundsZ = 0;
	float boundsW = 0;
	float boundsH = 0;
	public:
	void setBounds(float x, float y, float z, float w, float h){
		boundsX = x;
	        boundsY = y;
	        boundsZ = z;
	        boundsW = w;
	        boundsH = h;
	}

	void setText(string text){
		titleText = text;
	}

	void setFont(void *font){
		titleFont = font;
	}

	void setTextColor(float r, float g, float b){
		red = r;
		green = g;
		blue = b;
	}

	void setFontHeightOffset(float o){
		fontHeightOffset = o;
	}
	void setFontWidthOffset(float o){
		fontWidthOffset = o;
	}

	void run(){
		CoreObject::setColor(red, green, blue);
		float x = boundsX + (boundsW/2) - fontWidthOffset;
		float y = boundsY + (boundsH) - fontHeightOffset;
		float z = boundsZ + .001;
		CoreObject::setColor(red, green, blue);
		CoreObject::drawText(x, y, z, titleText, titleFont);
	}
};
