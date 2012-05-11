#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"

#include "gui.h"
#include "misc.h"
#include "memory.h"

extern int mouseX;
extern int mouseY;
extern bool mouse[3];
extern texture_t textures[256];
extern font_t fonts[256];
extern HDC hDC;
extern config cfg;

GUIcontrol::GUIcontrol()
{
	children = NULL;
	parent = NULL;

	childCount = 0;

	coordX = 0;
	coordY = 0;
	Z = Z_CTRL;
	parentCoordX = 0;
	parentCoordY = 0;
	sizeX = 0;
	sizeY = 0;
	name = NULL;

	tex = NULL;
	current_state = GUI_DEFAULT_STATE;

	captionFont = NULL;
	LPSTR text = NULL;

	textX = 0;
	textY = 0;

	textClrRed = 255;
	textClrGreen = 255;
	textClrBlue = 255;
	textClrAlpha = 255;

	visible = true;
}

GUIcontrol::GUIcontrol(GUIcontrol* prnt)
{
	GUIcontrol();
	parent = prnt;
	parentCoordX = prnt->coordX;
	parentCoordY = prnt->coordY;
	coordX = parentCoordX;
	coordY = parentCoordY;
}

GUIcontrol::Add(GUIcontrol ctrl)
{
	controls[childCount] = ctrl;


	childCount++;
}









//
//
//GUIcontrol::GUIcontrol()
//{
//	tex = NULL;
//	visible = true;
//	z = 0;
//	children = NULL;
//	countChildren = 0;
//	
//	tex_default = GUI_DEFAULT_STATE;
//	tex_mouseover = GUI_MOUSEOVER_STATE;
//	tex_press = GUI_PRESS_STATE;
//	tex_current_state = GUI_DEFAULT_STATE;
//
//
//}
//
//void GUIcontrol::Draw()
//{
//
//	int dx, dy;
//
//	if(visible)
//	{
//
//				glPushAttrib(GL_LINE_BIT);
//				glLineWidth(borderWidth);
//
//				if(AntiAlias)
//					glEnable(GL_LINE_SMOOTH);
//
//				if(tex)
//				{
//					dx = -1;
//					dy = 0;
//				}
//				else
//				{
//					dx = 0;
//					dy = 0;
//				}
//
//				if(border)
//					Box2(parentCoordX + coordX + dx,
//						parentCoordY + coordY + dy,
//						sizeX + 1,	sizeY + 1,
//						borderClrRed, borderClrGreen,
//						borderClrBlue, borderClrAlpha, 
//						borderClrRed2, borderClrGreen2,
//						borderClrBlue2, borderClrAlpha2);
//
//
//				if(body)
//					FilledBox2(parentCoordX + coordX,
//						parentCoordY + coordY,
//						sizeX,	sizeY,
//						bodyClrRed, bodyClrGreen,
//						bodyClrBlue, bodyClrAlpha,
//						bodyClrRed2, bodyClrGreen2,
//						bodyClrBlue2, bodyClrAlpha2);
//
//				if(AntiAlias)
//					glDisable(GL_LINE_SMOOTH);
//
//				glPopAttrib();
//
//				if(tex)
//				{			
//					DrawTexture(parentCoordX + coordX,
//						parentCoordY + coordY,
//						tex_current_state * tex_pattern_width,
//						tex_current_state * tex_pattern_height,
//						tex_pattern_width, tex_pattern_height,
//						tex,
//						false, false, 
//						sizeX / tex_pattern_width * 100 ,
//						sizeY / tex_pattern_height * 100);
//				}
//
//				if(caption_font && caption)
//				{
//					TextOut(string(caption), caption_font, &hDC,
//						parentCoordX + coordX + captX,
//						parentCoordY + coordY + captY,
//						captClrRed, captClrGreen, captClrBlue,
//						captClrAlpha);
//					// TODO: different colors & positions
//				}
//
//				if(font && text)
//				{
//					TextOut(string(text), font, &hDC,
//						parentCoordX + coordX + textX,
//						parentCoordY + coordY + textY,
//						textClrRed, textClrGreen, textClrBlue,
//						textClrAlpha);
//				}
//	}
//}
//
//void GUI::Tick()
//{
//	// TODO: static count of panels to overlay panel over previous
//
//	// TODO: sort by z-order expected
//
//
//	for(int i = 0; i < countPanel; i++)
//	{
//		for(int j = 0; j < panels[i].countButton; j++)
//		{
//			if(PointInBox(mouseX, mouseY,
//				panels[i].buttons[j].parentCoordX + panels[i].buttons[j].coordX,
//				panels[i].buttons[j].parentCoordY + panels[i].buttons[j].coordY,
//				panels[i].buttons[j].parentCoordX + panels[i].buttons[j].coordX + panels[i].buttons[j].sizeX,
//				panels[i].buttons[j].parentCoordY + panels[i].buttons[j].coordY,
//				panels[i].buttons[j].parentCoordX + panels[i].buttons[j].coordX,
//				panels[i].buttons[j].parentCoordY + panels[i].buttons[j].coordY + panels[i].buttons[j].sizeY,
//				panels[i].buttons[j].parentCoordX + panels[i].buttons[j].coordX + panels[i].buttons[j].sizeX,
//				panels[i].buttons[j].parentCoordY + panels[i].buttons[j].coordY + panels[i].buttons[j].sizeY))
//			{
//				//panels[i].buttons[j].tex = &textures[FindByName(textures, "button_green")];
//				panels[i].buttons[j].tex_current_state = GUI_MOUSEOVER_STATE;
//				if(mouse[0])
//				{
//					//panels[i].buttons[j].tex = &textures[FindByName(textures, "button_green_pressed")];
//					//panels[i].buttons[j].captX = panels[i].buttons[j].oldCaptX + 1;
//					//panels[i].buttons[j].captY = panels[i].buttons[j].oldCaptY + 1;
//					//DestroyPanel(i);
//					panels[i].buttons[j].tex_current_state = GUI_PRESS_STATE;
//				}
//				else
//				{
//					//panels[i].buttons[j].captX = panels[i].buttons[j].oldCaptX;
//					//panels[i].buttons[j].captY = panels[i].buttons[j].oldCaptY;
//					panels[i].buttons[j].tex_current_state = GUI_DEFAULT_STATE;
//				}
//				break;
//			}
//			else
//			{
//				//panels[i].buttons[j].tex = &textures[FindByName(textures, "button_black")];
//				//panels[i].buttons[j].captX = panels[i].buttons[j].oldCaptX;
//				//panels[i].buttons[j].captY = panels[i].buttons[j].oldCaptY;
//				panels[i].buttons[j].tex_current_state = GUI_DEFAULT_STATE;
//			}
//
//
//		}
//	}
//}
//
//
//GUI::GUI(): countPanel(0) { }
//
//void GUI::AddPanel(GUIpanel pnl)
//{
//	countPanel++;
//
//	panels[countPanel-1] = pnl;
//	panels[countPanel-1].countButton = 0;
//	panels[countPanel-1].countChildren = 0;
//	panels[countPanel-1].parentCoordX = 0;
//	panels[countPanel-1].parentCoordY = 0;
//	panels[countPanel-1].visible = true;
//	panels[countPanel-1].tex_pattern_width = panels[countPanel-1].tex->width / panels[countPanel-1].sizeX * panels[countPanel-1].tex->width ;
//	panels[countPanel-1].tex_pattern_height = panels[countPanel-1].tex->height / panels[countPanel-1].sizeY * panels[countPanel-1].tex->height;
//	
//}
//
//void GUI::DestroyPanel(int id)
//{
//	int i;
//	for(i = id; i < countPanel-1; i++)
//		panels[i] = panels[i+1];
//
//	//panels[i+1] = NULL;
//	countPanel--;
//}
//	
//
//void GUI::Draw()
//{
//	if (countPanel > 0)
//	{
//		for (int i = 0; i < countPanel; i++)
//		{
//			panels[i].Draw();
//
//			if(panels[i].countButton > 0)
//				for(int j = 0; j < panels[i].countButton; j++)
//					panels[i].buttons[j].Draw();
//		}
//	}
//}
//
//void GUI::ShowMessage(char message[], char caption[], GUIIcons icons)
//{
//
//	GUIpanel pnl;
//
//	GUIbutton btn;
//
//
//
//	pnl.tex = &textures[FindByName(textures,"panel")];
//	pnl.sizeX = pnl.tex->width;
//	pnl.sizeY = pnl.tex->height;
//	
//	pnl.coordX = cfg.scr_width / 2 - pnl.sizeX / 2;
//	pnl.coordY = cfg.scr_height / 2 - pnl.sizeY / 2;
//
//	pnl.body = false;
//	pnl.border = false;
//	pnl.caption_font = &fonts[1]; // Arial - 14
//	pnl.caption = caption;
//	pnl.captX = pnl.sizeX / 2 - strlen(pnl.caption) * 2 - 6;
//	pnl.captY = 5;
//	pnl.textClrAlpha = 255;
//	pnl.textClrBlue = 255;
//	pnl.textClrGreen = 255;
//	pnl.textClrRed = 255;
//	pnl.textX = 35;
//	pnl.textY = 25;
//	pnl.captClrAlpha = 255;
//	pnl.captClrBlue = 255;
//	pnl.captClrGreen = 255;
//	pnl.captClrRed = 255;
//	pnl.font = &fonts[0]; // Courier New - 10
//	pnl.text = message;
//		
//	AddPanel(pnl);
//
//
//	btn.tex = &textures[FindByName(textures, "button")];
//	btn.sizeX = btn.tex->width / GUI_STATE_COUNT;
//	btn.sizeY = btn.tex->height / GUI_STATE_COUNT;
//	//ShowNumber(btn.tex->height);
//	btn.coordX = pnl.sizeX / 2 - btn.sizeX / 2;
//	btn.coordY = pnl.tex->height - pnl.font->height - 25;
//	btn.body = false;
//	btn.border = false;
//	btn.caption_font = &fonts[1];
//	btn.caption = "Ok";
//	btn.text = NULL;
//	btn.font = NULL;
//	btn.captClrAlpha = 255;
//	btn.captClrBlue = 255;
//	btn.captClrGreen = 255;
//	btn.captClrRed = 255;
//
//	btn.captX = btn.sizeX / 2 - strlen(btn.caption) * 2;
//	btn.captY = 5;
//	btn.oldCaptX = btn.captX;
//	btn.oldCaptY = btn.captY;
//
//
//	panels[countPanel-1].AddButton(btn);
//
//	
//}
//	
//
//
//
//
//
//void GUIpanel::AddButton(GUIbutton btn)
//{
//	countButton++;
//	countChildren++;
//
//	buttons[countButton-1] = btn;
//	buttons[countButton-1].parentCoordX = coordX;
//	buttons[countButton-1].parentCoordY = coordY;
//	buttons[countButton-1].visible = true;
//
//
//	children = buttons;
//
//	buttons[countButton-1].tex_pattern_width = buttons[countButton-1].tex->width / buttons[countButton-1].sizeX * buttons[countButton-1].tex->width ;
//	buttons[countButton-1].tex_pattern_height = buttons[countButton-1].tex->height / buttons[countButton-1].sizeY * buttons[countButton-1].tex->height;
//
//}
//
//void GUIcontrol::Hide()
//{
//	visible = false;
//
//	for(int i = 0; i < countChildren; i++)
//	{
//		try
//		{
//			children[i].Hide();
//		}
//		catch(...)
//		{
//			LogToFile("Exception in GUIcontrol::Hide()");
//		}
//	}
//}
//
//
//
//
//
//
//
////bool GUI::LoadFromFile(char file_name[])
////{
////	int ok = 0;
////	string s;
////	string s_left;
////	string s_right;
////	int eq_pos = -1;
////	int lines = 0;
////	int params = 0;
////	string error;
////	
////
////	ifstream gui;
////
////	gui.open(file_name, ios::in);
////
////	if(!gui) return false;
////
////	while(!gui.eof())
////	{
////		gui >> s;
////		lines++;
////		// delete spaces		 
////		for(unsigned int i = 0; i < s.length(); i++){ if(s[i] == ' ') s.erase(i,1);}
////
////		// find '='
////		eq_pos = s.find('=');
////		
////		// if no '=' then error
////		if(eq_pos == s.npos)
////		{
////			error = string(" [line" + IntToString(lines) + "] : '=' expected");
////			MessageBox(NULL, error.c_str(), "Error reading config", MB_OK | MB_ICONEXCLAMATION);
////			return false;
////		}
////		else
////		{
////			s_left = s.substr(0,eq_pos);
////			s_right = s.substr(eq_pos+1,s.length()-1);
////
////			if(s_left.empty() || s_right.empty())
////			{
////				error = string(" [line" + IntToString(lines) + "] : Wrong string");
////				MessageBox(NULL, error.c_str(), "Error reading config", MB_OK | MB_ICONEXCLAMATION);
////				return false;
////			}
////			else
////			{
////				if(s_left == "scr_width") scr_width = atoi(s_right.c_str());
////				if(s_left == "scr_height") scr_height = atoi(s_right.c_str());
////				if(s_left == "bpp") scr_bpp = atoi(s_right.c_str());
////				if(s_left == "fullscreen") fullscreen = atoi(s_right.c_str()) !=0;
////				if(s_left == "vert_sync") vert_sync = atoi(s_right.c_str()) !=0;
////		
////			};
////					
////		}
////	}
////	return true;
