

enum GUI_ELEMENT_TYPE { 
	guiBaseControl,
	guiVidget,
	guiLabel,
	guiIcon,
	guiContainer,
	guiPanel,
	guiProgressBar,
	guiControl,
	guiButton,
	guiCheckBox,
	guiRadioButton,
	guiMemo,
	guiTextBox,
	guiNone
};


void NewGUIElem(string type, string name);
void SetGUIElem(string name, string field, string v1);
void SetGUIElem(string name, string field, string v1, string v2, string v3, string v4);
GUI_ELEMENT_TYPE osl_StrToGUI(string s);