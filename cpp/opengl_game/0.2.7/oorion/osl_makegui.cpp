#include "stdafx.h"

#include "font.h"
#include "texture.h"
#include "misc.h"
#include "config.h"
#include "draw.h"
#include "gui.h"
#include "osl_lexems.h"
#include "osl_misc.h"

#include "osl_makegui.h"


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
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Creating new GUI element...");
	GUI_ELEMENT_TYPE t = osl_StrToGUI(type);

	switch(t)
	{
	case guiBaseControl:
		{
			char* n = new char[strlen((char*)name.data())+1];
			sprintf(n, "%s\0", name.data());
			GUIBaseControl* ge = new GUIBaseControl(n);	
			gui->GetControls()->Add(ge);
			break;
		}
	case guiVidget:
		{
			char* n = new char[strlen((char*)name.data())+1];
			sprintf(n, "%s\0", name.data());
			GUIVidget* ge = new GUIVidget(n);		
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
			char* n = new char[strlen((char*)name.data())+1];
			sprintf(n, "%s\0", name.data());
			GUIControl* ge = new GUIControl(n);	
			gui->GetControls()->Add(ge);
			break;
		}
	case guiButton:
		{
			char* n = new char[strlen((char*)name.data())+1];
			sprintf(n, "%s\0", name.data());
			GUIButton* ge = new GUIButton(n);
			gui->GetControls()->Add(ge);
			break;
		}
	case guiTextBox:
		{
			//char* n = new char[strlen((char*)name.data())+1];
			//sprintf(n, "%s\0", name.data());
			//GUITextBox* ge = new GUITextBox(n);	
			//gui->GetControls()->Add(ge);
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
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s, %s, %s, %s)",
		name.data(), field.data(), v1.data(), v2.data(), v3.data(), v4.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);
	if(field == "Color")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
	else if(field == "CaptionColor")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
			(char*)name.data());

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
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s, %s, %s)",
		name.data(), field.data(), v1.data(), v2.data(), v3.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);

	if(field == "AddSignal")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
	char output[300];

	sprintf(output, "Setting GUI element... (%s, %s, %s)",
		name.data(), field.data(), v1.data());

	WriteToLog(DEFAULT_OSL_LOG_NAME, output);

	if(field == "Width")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
			(char*)name.data());

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
			(char*)name.data());

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
			(char*)name.data());

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
			(char*)name.data());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			texture_t* tex = TextureByName(n);
			texture_t* tex2 = TextureByName("button_long");

			if(tex)
				ctrl->SetTexture(tex);
			else
				osl_Error("SetGUIElem: texture not found");
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "Visible")
	{
		GUIVidget* ctrl = (GUIVidget*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
	else if(field == "Caption")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			(char*)name.data());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			ctrl->SetCaption(n);
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "CaptionFont")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			(char*)name.data());

		if(ctrl)
		{
			char* n = new char[strlen((char*)v1.data())+1];
			sprintf(n, "%s\0", v1.data());
			font_t* font = FontByName(n);

			ctrl->SetCaptionFont(FontByName((char*)v1.data()));

			//delete[] n;
		}
		else
			osl_Error("SetGUIElem: no such GUI control");
	}
	else if(field == "PatternCount")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
			(char*)name.data());

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
			(char*)name.data());

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
	//else if(field == "Sgn")
	//{
	//	GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
	//		(char*)name.data());

	//	if(ctrl)
	//	{
	//		if(osl_IsIntConstant(v1))
	//			ctrl->SetSgn(osl_StrToInt(v1));
	//		else
	//			osl_Error("SetGUIElem: incompatible types");
	//	}
	//	else
	//		osl_Error("SetGUIElem: no such GUI control");
	//}
	//else if(field == "SgnParam1")
	//{
	//	GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
	//		(char*)name.data());

	//	if(ctrl)
	//	{
	//		if(osl_IsIntConstant(v1))
	//			ctrl->SetSgnParam1(osl_StrToInt(v1));
	//		else
	//			osl_Error("SetGUIElem: incompatible types");
	//	}
	//	else
	//		osl_Error("SetGUIElem: no such GUI control");
	//}
	//else if(field == "SgnParam2")
	//{
	//	GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
	//		(char*)name.data());

	//	if(ctrl)
	//	{
	//		if(osl_IsIntConstant(v1))
	//			ctrl->SetSgnParam2(osl_StrToInt(v1));
	//		else
	//			osl_Error("SetGUIElem: incompatible types");
	//	}
	//	else
	//		osl_Error("SetGUIElem: no such GUI control");
	//}
	//else if(field == "Data1")
	//{
	//	GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
	//		(char*)name.data());

	//	if(ctrl)
	//	{
	//		if(osl_IsIntConstant(v1))
	//			ctrl->SetData1(osl_StrToInt(v1));
	//		else
	//			osl_Error("SetGUIElem: incompatible types");
	//	}
	//	else
	//		osl_Error("SetGUIElem: no such GUI control");
	//}
	//else if(field == "Data2")
	//{
	//	GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByName(
	//		(char*)name.data());

	//	if(ctrl)
	//	{
	//		if(osl_IsIntConstant(v1))
	//			ctrl->SetData2(osl_StrToInt(v1));
	//		else
	//			osl_Error("SetGUIElem: incompatible types");
	//	}
	//	else
	//		osl_Error("SetGUIElem: no such GUI control");
	//}
	else if(field == "Parent")
	{
		GUIBaseControl* ctrl = (GUIBaseControl*)gui->GetControls()->
			GetControlByName((char*)name.data());

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
			GetControlByName((char*)name.data());

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
	else if(field == "CaptionX")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->
			GetControlByName((char*)name.data());

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
	else if(field == "CaptionY")
	{
		GUIControl* ctrl = (GUIControl*)gui->GetControls()->
			GetControlByName((char*)name.data());

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