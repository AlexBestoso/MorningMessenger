#include "./formBackground.h"
#include "./formTitle.h"
#include "./formText.h"
#include "./formButton.h"
#include "./formTextInput.h"
#include "./formDropDown.h"
#include "./formSelectList.h"
class Form : public CoreObject{
	private:
	int formContext = 0;
	FormBackground background;
	
	FormTitle title;
	
	FormText *texts = NULL;
	size_t textCount = 0;

	FormButton *buttons = NULL;
	size_t buttonCount = 0;
	
	FormTextInput *textInputs = NULL;
	size_t textInputCount = 0;
	
	FormDropDown *dropDowns = NULL;
	size_t dropDownCount = 0;

	FormSelectList *selectLists = NULL;
	size_t selectListCount = 0;
	
	bool hideTitle = false;

	int clickedButtonId = -1;

	int clickedDropdownId = -1;
	int clickedDropdownOptionId = -1;

	int clickedSelectListId = -1;
        int clickedSelectListOptionId = -1;

	public:
	Form(FormBackground background){
		this->background = background;
	}
	Form(){
		
	}
	
	int getClickedButton(){
		return clickedButtonId;
	}
	int getClickedDropDown(){
		return clickedDropdownId;
	}
	int getClickedDropDownOption(){
		return clickedDropdownOptionId;
	}
	int getClickedSelectList(){
                return clickedSelectListId;
        }
        int getClickedSelectListOption(){
                return clickedSelectListOptionId;
        }

	void setContext(int context){
		this->formContext = context;
	}
	int getContext(void){
		return formContext;
	}
	
	void setBackground(FormBackground background){
		this->background = background;
	}
	void setTitle(FormTitle title){
		this->title = title;

		this->title.setBounds(
			this->background.getBgX(),		
			this->background.getBgY(),		
			this->background.getBgZ(),		
			this->background.getBgW(),		
			this->background.getBgH()
		);
	}

	void setButtons(FormButton *buttons, size_t buttonCount){
		this->buttons = buttons;
		this->buttonCount = buttonCount;
	}

	void setTexts(FormText *texts, size_t textCount){
		this->texts = texts;
		this->textCount = textCount;
	}

	void setTextInputs(FormTextInput *inputs, size_t inputCount){
		this->textInputCount = inputCount;
		this->textInputs = inputs;
	}
	void setDropDowns(FormDropDown *drops, size_t count){
		this->dropDowns = drops;
		this->dropDownCount = count;
	}

	void setSelectLists(FormSelectList *lists, size_t count){
		this->selectLists = lists;
		this->selectListCount = count;
	}

	void setHideTitle(bool hide){
		hideTitle = hide;
	}

	virtual int run(){
		background.run();
		if(!hideTitle)
			title.run();
		for(int i=0; i<buttonCount; i++){
			buttons[i].run();
		}
		for(int i=0; i<textInputCount; i++){
			textInputs[i].run();
                }
		for(int i=0; i<dropDownCount; i++){
			dropDowns[i].run();
		}
		for(int i=0; i<selectListCount; i++){
			selectLists[i].run();
		}
		for(int i=0; i<textCount; i++){
			if(!texts[i].getHideText())
				texts[i].run();
		}
		return this->formContext;
	}

	virtual int mousePassive(float x, float y){
		for(int i=0; i<buttonCount; i++){
                        buttons[i].mousePassive(x, y);
                }
		for(int i=0; i<textInputCount; i++){
			textInputs[i].mousePassive(x, y);
		}
		for(int i=0; i<dropDownCount; i++){
			dropDowns[i].mousePassive(x, y);
		}
		for(int i=0; i<selectListCount; i++){
                        selectLists[i].mousePassive(x, y);
                }
		return this->formContext;
	}

	void resetAllClickIds(){
		clickedButtonId = -1;
		clickedDropdownId = -1;
                clickedDropdownOptionId = -1;
		clickedSelectListId = -1;
                clickedSelectListOptionId = -1;
	}
	virtual int mouseClick(int button, int state, float x, float y){
		resetAllClickIds();
		for(int i=0; i<buttonCount; i++){
                        int val = buttons[i].mouseClick(button, state, x, y);
			if(val != -1){
				clickedButtonId = i;
				return val;
			}
                }
		for(int i=0; i<textInputCount; i++){
			textInputs[i].mouseClick(button, state, x, y);	
                }

		for(int i=0; i<dropDownCount; i++){
			int val = dropDowns[i].mouseClick(button, state, x, y);
			if(val != -1){
				clickedDropdownId = i;
				clickedDropdownOptionId = dropDowns[i].getClickedOption();
				return val;
			}
		}
		for(int i=0; i<selectListCount; i++){
                        int val = selectLists[i].mouseClick(button, state, x, y);
			if(val != -1){
				clickedSelectListId = i;
				clickedSelectListOptionId = selectLists[i].getClickedOption();
				return val;
			}
                }
                return -1;
	}

	virtual int keyDown(unsigned char key, float x, float y){
		for(int i=0; i<textInputCount; i++){
			textInputs[i].keyDown(key, x, y);
		}
		return -1;
	}
};
