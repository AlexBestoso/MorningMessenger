class FormBackground : public CoreObject{
	private:
		float x=-0.50;
		float y=-0.50;
		float z=0;
		float w=1;
		float h=1;

		float red = .25;
		float green = .75;
		float blue = .75;

		bool useTexture = false;
		string texture = "";
	public:
	float getBgX(){
		return x;
	}
	float getBgY(){
		return y;
	}
	float getBgZ(){
		return z;
	}
	float getBgH(){
		return h;
	}
	float getBgW(){
		return w;
	}
	void setUseTexture(bool use){
		useTexture = use;
	}
	void setTexturePath(string tex){
		texture = tex;
	}
	
	void setCoords(float _x, float _y, float _z){
		x = _x;
		y = _y;
		z = _z;
	}

	void setWidthHeight(float _w, float _h){
		w = _w;
		h = _h;
	}

	void setColor(float r, float g, float b){
		red = r;
		green = g;
		blue = b;
	}
	void run(void){
		CoreObject::setColor(red, green, blue);
		if(useTexture){
			drawTexturedRectangle(texture, getX(x), getY(y), z, w, h);
		}else{
			CoreObject::drawRectangle(x, y, z, w, h);
		}
	}	
};
