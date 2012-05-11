#include "stdafx.h"

#include "../draw/font.h"
#include "../draw/texture.h"
#include "../misc.h"
#include "../config.h"
#include "../draw/draw.h"
#include "../gui/gui.h"

#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_misc.h"

#include "osl_makegui.h"


#include "../gui/gui_misc.h"
#include "../gui/gui_msgalloc.h"
#include "../gui/gui_ctrlalloc.h"
#include "../gui/gui_basecontrol.h"
#include "../gui/gui_vidget.h"
#include "../gui/gui_control.h"
#include "../gui/gui_button.h"
#include "../gui/gui_textbox.h"


extern GUI* gui;

GUI_ELEMENT_TYPE osl_StrToGUI(string s)
{
	if(s == "GUIBaseControl")
		return guiBaseControl;
	else if(s == "GUIVidget")
		return guiVidget;
	else if(s == "GUILabel")
		return guiLabel;
	else if(s == "GUIIcon")
		return guiIcon;
	else if(s == "GUIContainer")
		return guiContainer;
	else if(s == "GUIPanel")
		return guiPanel;
	else if(s == "GUIProgressBar")
		return guiProgressBar;
	else if(s == "GUIControl")
		return guiControl;
	else if(s == "GUIButton")
		return guiButton;
	else if(s == "GUICheckBox")
		return guiCheckBox;
	else if(s == "GUIRadioButton")
		return guiRadioButton;
	else if(s == "GUIMemo")
		return guiMemo;
	else if(s == "GUITextBox")
		return guiTextBox;
	else return guiNone;
}





void NewGUIElem(string type, string name)
{
	#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Creating new GUI element...");
	#endif
	GUI_ELEMENT_TYPE t = osl_StrToGUI(type);

	if(!gui)
		return;

	switch(t)
	{
	case guiBaseControl:
		{
			//char* n = new char[name.length()+1];
			//sprintf(n, "%s", name.c_str());
			GUIBaseControl* ge = new GUIBaseControl(name);
			gui->GetControls()->Add(ge);
			break;
		}
	case guiVidget:
		{
			//char* n = new char[name.length()+1];
			//sprintf(n, "%s", name.c_str());
			GUIVidget* ge = new GUIVidget(name);

			gui->GetControls()->Add(ge);

			break;
		}
	case guiLabel:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUILabel* ge = new GUILabel(n);			
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiIcon:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUITextBox* ge = new GUITextBox(n);			
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiContainer:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUIContainer* ge = new GUIContainer(n);		
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiPanel:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUIPanel* ge = new GUIPanel(n);		
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiProgressBar:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUIProgressBar* ge = new GUIProgressBar(n);		
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiControl:
		{
			//char* n = new char[name.length()+1];
			//sprintf(n, "%s", name.c_str());
			GUIControl* ge = new GUIControl(name);
			gui->GetControls()->Add(ge);
			break;
		}
	case guiButton:
		{
			//char* n = new char[name.length()+1];
			//sprintf(n, "%s", name.c_str());
			GUIButton* ge = new GUIButton(name);
			gui->GetControls()->Add(ge);
			break;
		}
	case guiTextBox:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			GUITextBox* ge = new GUITextBox(name);	
			gui->GetControls()->Add(ge);
			break;
		}
	case guiMemo:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUIMemo* ge = new GUIMemo(n);
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiRadioButton:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUIRadioButton* ge = new GUIRadioButton(n);	
			//gui->GetControls()->Add(ge);
			break;
		}
	case guiCheckBox:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUICheckBox* ge = new GUICheckBox(n);	
			//gui->GetControls()->Add(ge);
			break;
		}
	default:
		{
			osl_Warning("Trying to add unknown GUI element");
			return;
		}
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "New GUI control: " + type + "("
		+ name + ")");
}

void SetGUIElem(string name, string field, string v1, string v2, string v3, string v4)
{
	if(!gui)
		return;
	#ifdef _DEBUG_OSL
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s, %s, %s, %s)",
		name.data(), field.data(), v1.data(), v2.data(), v3.data(), v4.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);
	#endif
	if(field == "TextColor")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetTextColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Color")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "BackgroundColor")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetBackgroundColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CaptionColor")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetCaptionColor(c);
				ctrl->SetCurrentCaptionColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "SelectedCaptionColor")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetSelectedCaptionColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "PressedCaptionColor")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3) &&
				osl_IsIntConstant(v4))
			{
				COLOR4 c;
				c.r = osl_StrToInt(v1);
				c.g = osl_StrToInt(v2);
				c.b = osl_StrToInt(v3);
				c.a = osl_StrToInt(v4);
				ctrl->SetPressedCaptionColor(c);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else
		osl_Error("SetGUIElem: incompatible command and parametres");
}

void SetGUIElem(string name, string field, string v1, string v2, string v3)
{
	if(!gui)
		return;
#ifdef _DEBUG_OSL
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s, %s, %s)",
		name.data(), field.data(), v1.data(), v2.data(), v3.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);
#endif

	if(field == "AddSignal")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1) &&
				osl_IsIntConstant(v2) &&
				osl_IsIntConstant(v3))

				ctrl->AddSgn(osl_StrToInt(v1),
					osl_StrToInt(v2),
					osl_StrToInt(v3));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else
		osl_Error("SetGUIElem: unknown field");
}




void SetGUIElem(string name, string field, string v1)
{

	if(!gui)
		return;
 #ifdef _DEBUG_OSL
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s)",
		name.data(), field.data(), v1.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);
#endif

	if(field == "Width")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetWidth(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Height")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetHeight(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "X")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetX(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Y")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetY(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Texture")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			Texture* tex = TextureByName(n);

			if(tex)
				ctrl->SetTexture(tex);
			else
				osl_Error("SetGUIElem: texture not found");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Border")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
			{
				bool f = (osl_StrToInt(v1) == 1);
				ctrl->SetBorder(f);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIelem: no such GUI control");
	}
	else if(field == "BorderType")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
			{
				BORDER_TYPE type = BORDER_TYPE(osl_StrToInt(v1));
				ctrl->SetBorderType(type);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIelem: no such GUI control");
	}
	else if(field == "Visible")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
			{
				bool f = (osl_StrToInt(v1) == 1);
				ctrl->SetVisible(f);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Text")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			ctrl->SetText(n);
			//ctrl->SetText(v1);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Caption")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			ctrl->SetCaption(n);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "TextFont")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			Font* font = FontByName((char*)v1.c_str());
			if(font)
				ctrl->SetTextFont(font);
			else
				osl_Warning("SetGUIElem(): no such font: " + v1);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CaptionFont")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			Font* font = FontByName((char*)v1.c_str());
			if(font)
				ctrl->SetCaptionFont(font);
			else
				osl_Warning("SetGUIElem(): no such font: " + v1);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "PatternCount")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetPatternCount(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CurrentPattern")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetCurrentPattern(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Parent")
	{
		GUIBaseControl* ctrl = (GUIBaseControl*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			ctrl->SetParent(n);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Group")
	{
		GUIBaseControl* ctrl = (GUIBaseControl*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetGroup(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "TextX")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetTextX(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "TextY")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetTextY(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CaptionX")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetCaptionX(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Enabled")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
			{
				bool f = (osl_StrToInt(v1) == 1);
				ctrl->SetEnabled(f);
			}
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CaptionY")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
		{
			if(osl_IsIntConstant(v1))
				ctrl->SetCaptionY(osl_StrToInt(v1));
			else
				osl_Error("SetGUIElem: incompatible types");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else
		osl_Error("SetGUIElem: unknown field");
}



string GetGUIElem(string name, string field)
{
	if(field == "Text")
	{
		GUITextBox* ctrl = (GUITextBox*)gui->GetControls()->
			GetControlByName(name.c_str());

		if(ctrl)
			return ctrl->GetText();
		else
			osl_Error("GetGUIElem: no such GUI control");
	}
	else
		osl_Error("GetGUIElem: unknown field");
	return "";
}