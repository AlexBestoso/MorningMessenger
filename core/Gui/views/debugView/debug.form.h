class DebugForm : public Form{
	private:
		FormBackground background;
		FormScrollText scrollText[1];
		MorningSetup setup;
	public:
		DebugForm() : Form(){
			if(!setup.isSetup()){
                                return;
                        }
			// Setup background
			background.setUseTexture(false);
			Form::setBackground(background);
			Form::setHideTitle(true);

			// Test Scroll Text
			scrollText[0].setMenuColor(1, 1, 1);
			scrollText[0].setMenuWidthHeight(background.getBgW(), background.getBgH());
			scrollText[0].setMenuCoords(
				background.getBgX(),
				background.getBgY(),
				0		
			);
			scrollText[0].setEscapeText(true);
			scrollText[0].setMaxCols(17);
			string test = "";
			string modifyer = "A";
			for(int i=0; i<16*26; i++){
				if(i!=0 && (i%16) == 0){
					modifyer = modifyer[0]+1;
					test += "\n";
				}
				test += modifyer;
			}
			scrollText[0].setText(test);
			scrollText[0].setTextCoords(
				background.getBgX()+(background.getBgW()/3),
				background.getBgY() + background.getBgH()-0.08,
				0		
			);
			
			Form::setScrollTexts(scrollText, 1);
		}
};
