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

	void setFocus(bool val){
		focused = val;
	}

	bool getFocus(void){
		return focused;
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

	string getClipboardText(void){
		string ret = "";
		Display *display = XOpenDisplay(NULL);
  		unsigned long color = BlackPixel(display, DefaultScreen(display));
  		Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);
		const char *bufname = "CLIPBOARD";
		const char *fmtname = "UTF8_STRING";
		char *result;
  		unsigned long ressize, restail;
  		int resbits;
  		Atom bufid = XInternAtom(display, bufname, False);
       		Atom fmtid = XInternAtom(display, fmtname, False);
       		Atom propid = XInternAtom(display, "XSEL_DATA", False);
       		Atom incrid = XInternAtom(display, "INCR", False);
  		XEvent event;

  		XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
  		do {
    			XNextEvent(display, &event);
  		} while (event.type != SelectionNotify || event.xselection.selection != bufid);

  		if (event.xselection.property){
    			XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
      			&fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

    			if (fmtid == incrid){
      				printf("Buffer is too large and INCR reading is not implemented yet.\n");
			}else{
      				ret = result;
			}	
    			XFree(result);
    			return ret;
  		}
		// request failed, e.g. owner can't convert to the target format
    		return ret;
	}

	string getClipboardText2(void){
                string ret = "";
                Display *display = XOpenDisplay(NULL);
                unsigned long color = BlackPixel(display, DefaultScreen(display));
                Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);
                const char *bufname = "CLIPBOARD";
                const char *fmtname = "STRING";
                char *result;
                unsigned long ressize, restail;
                int resbits;
                Atom bufid = XInternAtom(display, bufname, False);
                Atom fmtid = XInternAtom(display, fmtname, False);
                Atom propid = XInternAtom(display, "XSEL_DATA", False);
                Atom incrid = XInternAtom(display, "INCR", False);
                XEvent event;

                XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
                do {
                        XNextEvent(display, &event);
                } while (event.type != SelectionNotify || event.xselection.selection != bufid);

                if (event.xselection.property){
                        XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
                        &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

                        if (fmtid == incrid){
                                printf("Buffer is too large and INCR reading is not implemented yet.\n");
                        }else{
                                ret = result;
                        }
                        XFree(result);
                        return ret;
                }
                // request failed, e.g. owner can't convert to the target format
                return ret;
        }

	bool setClipboardValue(string dest){
		if(dest.length() <= 0){
			return false;
		}
		// init X11 display and window.
		Display *display = XOpenDisplay(NULL);
  		unsigned long color = BlackPixel(display, DefaultScreen(display));
  		Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);

		// Copy string into an unsigned char array, may be able to just typecast.
		size_t destSize = dest.length();
		unsigned char *copyData = new unsigned char[destSize];
		int copyDataLen = destSize;
		for(int i=0; i<copyDataLen; i++){
			copyData[i] = (unsigned char)dest[i];
		}
	
		// Pre-iteration preperations...
		XEvent event;
		Atom selection = XA_CLIPBOARD(display);
		Atom target = XA_UTF8_STRING(display);
	
		XSelectInput(display, window, PropertyChangeMask);
		XSetSelectionOwner(display, selection, window, CurrentTime);

		Window requestor_id;

		/*
		 * Loop over a few events.
		 * The first 3 appears to require sending information regarding the previous event to the X11 Server.
		 * On the 4th iteration the event that allows you to write into the clipboard appears to become usable. 
		 * */
		for(int i=0; i<4; i++){
			XNextEvent(display, &event);
			if(event.type == SelectionRequest){
				requestor_id = event.xselectionrequest.requestor;
			}else{
				XDestroyWindow(display, window);
			 	XCloseDisplay(display);
				return false;
			}
		
			XEvent eventResponse;
	    		Atom inc;
	    		Atom targets;
			targets = XInternAtom(display, "TARGETS", False);
			inc = XInternAtom(display, "INCR", False);
	
			if (event.xselectionrequest.target == targets) {
				// These operations will allow the code to continue iterating through events.
				Atom types[2] = { targets, XA_UTF8_STRING(display) };
				XChangeProperty(display,
		        	                event.xselectionrequest.requestor,
		        	                event.xselectionrequest.property,
		        	                XA_ATOM,
		        	                32, PropModeReplace, (unsigned char *) types,
		        	                (int) (sizeof(types) / sizeof(Atom))
		        	);
			}else{
				// This is what actually sets the clipboard data.
				XChangeProperty(display,
		                            event.xselectionrequest.requestor,
		                            event.xselectionrequest.property,
		                            XA_UTF8_STRING(display),
		                            8, PropModeReplace, (unsigned char *) copyData,
		                            (int) copyDataLen);
			}
		
			// Create a X11 event packet and send it to the server.
			eventResponse.xselection.property = event.xselectionrequest.property;
		        eventResponse.xselection.type = SelectionNotify;
		        eventResponse.xselection.display = event.xselectionrequest.display;
		        eventResponse.xselection.requestor = event.xselectionrequest.requestor;
		        eventResponse.xselection.selection = event.xselectionrequest.selection;
		        eventResponse.xselection.target = event.xselectionrequest.target;
		        eventResponse.xselection.time = event.xselectionrequest.time;
		
			XSendEvent(display, event.xselectionrequest.requestor, 0, 0, &eventResponse);
		        XFlush(display);
		}
		// Clean up.
		XDestroyWindow(display, window);
	  	XCloseDisplay(display);
		return true;
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
			if(key == 0x08){ // backspace
				if(this->inputData.size() > 0)
					this->inputData.pop_back();
			}else if(key == 0x0d){ // Enter key
                                this->focused = false;
                        }else if(dirSafeInput){ // exclude certain chars
				if((key >= 0x61 && key <= 0x7a) || (key >= 0x41 && key <= 0x5a) || (key >= 0x30 && key <= 0x39) || key == 0x20){
					this->inputData += key;
				}
			}else if(key == 0x16){ // Ctrl+v / paste
				this->inputData = getClipboardText();
				if(this->inputData == ""){
					this->inputData = getClipboardText2();
				}
			}else if(key == 0x3){ // Ctrl+c / copy
				string oldVal = this->inputData;
				int e = 0;
				int eMax = 10;
				while(oldVal != getClipboardText() || e < eMax){
					setClipboardValue(this->inputData);
					e++;
				}
			}else if(key == 0x18){ // Ctrl+x / cut
				string oldVal = this->inputData;
                                int e = 0;
                                int eMax = 10;
                                while(oldVal != getClipboardText() || e < eMax){
                                        setClipboardValue(this->inputData);
                                        e++;
                                }
				this->inputData = "";
			}else if(!dirSafeInput){
				this->inputData += key;
			}
		}
		
		return 0;
	}
};
