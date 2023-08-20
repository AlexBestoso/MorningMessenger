class View{
        private:
		bool initalized = false;
                int _viewId = 0;
                float _bgColorRed = .25;
                float _bgColorGreen = 0;
                float _bgColorBlue = .5;
                float _bgColorAlpha = 1;

                void setBackgroundColor(){
                        glClearColor(
                                _bgColorRed,
                                _bgColorGreen,
                                _bgColorBlue,
                                _bgColorAlpha
                        );
                        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                }
        public:
		View(){}
	
        	View(int viewId, float bgColor[4]){
        	        _viewId = viewId;
        	        _bgColorRed = bgColor[0];
        	        _bgColorGreen = bgColor[1];
        	        _bgColorBlue = bgColor[2];
        	        _bgColorAlpha = bgColor[3];
        	}


		int getViewId(void){
        	        return _viewId;
        	}
        	void setViewId(int id){
        	        _viewId = id;
        	}

        	void setBgColor(float bgColor[4]){
        	        _bgColorRed = bgColor[0];
        	        _bgColorGreen = bgColor[1];
        	        _bgColorBlue = bgColor[2];
        	        _bgColorAlpha = bgColor[3];
        	}
        	void getBgColor(float bgColor[4]){
        	        bgColor[0] = _bgColorRed;
        	        bgColor[1] = _bgColorGreen;
        	        bgColor[2] = _bgColorBlue;
        	        bgColor[3] = _bgColorAlpha;
        	}

		virtual bool isInit(void){
			return initalized;
		}

		virtual void init(int viewId, float bgColor[4]){
			_viewId = viewId;
                        _bgColorRed = bgColor[0];
                        _bgColorGreen = bgColor[1];
                        _bgColorBlue = bgColor[2];
                        _bgColorAlpha = bgColor[3];	
			initalized = true;
		}
		virtual int run(){
        	        setBackgroundColor();
        	        return _viewId;
        	}

        	virtual int mouseClick(int button, int state, float x, float y){
        	        setBackgroundColor();
        	        return _viewId;
        	}

        	virtual int mousePassive(float x, float y){
        	        setBackgroundColor();
        	        return _viewId;
        	}

        	virtual int keyDown(unsigned char key, int mouseX, int mouseY){
        	        setBackgroundColor();
        	        return _viewId;
        	}
        	virtual int keyUp(unsigned char key, int mouseX, int mouseY){
        	        setBackgroundColor();
        	        return _viewId;
        	}
		virtual int specialKeyDown(int key, int mouseX, int mouseY){
			return -1;
		}
                virtual int specialKeyUp(int key, int mouseX, int mouseY){
			return -1;
		}
};
