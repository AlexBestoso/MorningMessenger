class FormScrollText : public CoreObject{
	private:
	float menuX = 0;
	float menuY = 0;
	float menuZ = 0;
	float menuW = .3;
	float menuH = .3;
	float menuR = 0;
	float menuG = 0;
	float menuB = 0;

	float textX = 0;
	float textY = 0;
	float textZ = 0;
	float textR = 0;
	float textG = 0;
	float textB = 0;

	bool hovered = false;
	bool escapeSpecials = false;

	void* textFont = GLUT_BITMAP_TIMES_ROMAN_24;
	string textBuffer = "";
	string displayBuffer = "";
	string *visibleBuffer = NULL;
	int *rowStartPos = NULL;
	int *trueRowLen = NULL;
	size_t maxVisibleRows = 16;
	size_t maxVisibleCols = 16;
	int topLine = 0;
	bool customNewLines = false;
	int totalLines = 0;
	
	void updateVisibleBuffer(void){
		if(textBuffer.length() == 0){
			return;
		}

		if(visibleBuffer == NULL)
			visibleBuffer = new string[maxVisibleRows];
		if(rowStartPos == NULL)
			rowStartPos = new int[maxVisibleRows];
		if(trueRowLen == NULL)
			trueRowLen = new int[maxVisibleRows];

		for(int i=0; i<maxVisibleRows; i++)
			visibleBuffer[i] = "";
		char *lineGrabber = new char[maxVisibleCols+1];
		for(int j=0; j<maxVisibleCols+1; j++)
                                        lineGrabber[j] = 0x00;
		
		int line_i = 0;
		int startPos_i = 0;
		int lineBuf_i = 0;
		int trueLen_i = 0;
		int startPos = 0;
		// Find the starting position for parsing.
		if(topLine > 0){
			int track = 0;
			bool found = false;
			for(int i=0; i<textBuffer.length(); i++){
				if(track >= topLine){
					startPos = i;
					found = true;
					break;
				}
				if(textBuffer[i] == '\n' || (i!=0 && (i%maxVisibleCols) == 0)){
					track++;
				}
			}
			if(!found){
				startPos = (textBuffer.length()-1) - maxVisibleCols;
			}
		}

		// populate visble buffer
		rowStartPos[startPos_i] = startPos;
		bool valNeedsSet = true;
		for(int i=startPos; i<textBuffer.length() && lineBuf_i < maxVisibleRows; i++){
			lineGrabber[line_i] = textBuffer[i];
			line_i++;
			valNeedsSet = true;
			if(!(line_i < maxVisibleCols) || textBuffer[i] == '\n'){
				visibleBuffer[lineBuf_i] = lineGrabber;
				
				trueRowLen[trueLen_i] = visibleBuffer[lineBuf_i].length();

				if(textBuffer[i] != '\n')
					visibleBuffer[lineBuf_i]+= "\n";
				
				for(int j=0; j<maxVisibleCols; j++)
					lineGrabber[j] = 0x00;
				
				line_i = 0;
				lineBuf_i++;
				startPos_i++;
				trueLen_i++;
				if(startPos_i < maxVisibleRows)
					rowStartPos[startPos_i] = i;
				valNeedsSet = false;
			}
		}
		if(valNeedsSet){
			visibleBuffer[lineBuf_i] = lineGrabber;
		}

		// Apply to display buffer
		displayBuffer = "";
		for(int i=0; i<maxVisibleRows; i++){
			if(visibleBuffer[i] != "")
			displayBuffer += visibleBuffer[i];
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

	string getText(void){
		return textBuffer;
	}

	void setMaxRows(size_t val){
		maxVisibleRows = val;
	        if(visibleBuffer != NULL){
			delete[] visibleBuffer;
		}
	}
        void setMaxCols(size_t val){
        	maxVisibleCols = val;
	}
	
	void setText(string val){
		if(!escapeSpecials){
			textBuffer = val;
		}else{
			textBuffer = "";
			for(int i=0; i<val.length(); i++){
				if(val[i] == '\n'){
					textBuffer += "\\n";
				}else if(val[i] == '\t'){
					textBuffer += "\\t";
				}else if(val[i] == '\r'){
					textBuffer += "\\r";
				}else{
					textBuffer += val[i];
				}
			}
		}
		int tracker = 0;
		totalLines = 0;
		for(int i=0; i<textBuffer.length(); i++){
			if(textBuffer[i] == '\n'){
				totalLines++;
				tracker=0;
				continue;
			}else if(tracker >= maxVisibleCols){
				totalLines++;
				tracker = 0;
				continue;
			}
			tracker++;
		}
		updateVisibleBuffer();
	}

	void setEscapeText(bool val){
		escapeSpecials = val;
		if(textBuffer != "" && escapeSpecials){
			setText(textBuffer);
		}
	}

	void setTextFont(int fontId){
		textFont = CoreObject::getFontById(fontId);
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

	void setMenuCoords(float x, float y, float z){
		menuX = x;
		menuY = y; 
		menuZ = z;
	}

	void setTextCoords(float x, float y, float z){
		textX = x;
		textY = y; 
		textZ = z;
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
	void setTextColor(float r, float g, float b){
		textR = r;
		textG = g;
		textB = b;
	}

	void run(void){
		CoreObject::setColor(menuR, menuG, menuB);
		CoreObject::drawRectangle(menuX, menuY, menuZ, menuW, menuH);
		CoreObject::setColor(textR, textG, textB);
		CoreObject::drawText(textX, textY, textZ, displayBuffer, textFont);

	}

	int mousePassive(float x, float y){
		activelyHovered(x, y);
		return -1;
	}
	
	int mouseClick(int button, int state, float x, float y){
		if(hovered){
			if(button == 4 && state == 1){
                        	topLine++;
				if(topLine > totalLines)
					topLine = totalLines;
				updateVisibleBuffer();
                        }else if(button == 3 && state == 1){
                             	topLine--;
				if(topLine < 0)
					topLine = 0;
				updateVisibleBuffer();
                       	}
		}
		return -1;
	}

	int keyDown(unsigned char key, int mouseX, int mouseY){
        	return -1;
	}
	int specialKeyDown(int key, int mouseX, int mouseY){
		return -1;
	}
	int specialKeyUp(int key, int mouseX, int mouseY){
		return -1;
	}
	int keyUp(unsigned char key, int mouseX, int mouseY){
		return -1;
	}
	void idle(void){
	
	}
};
