#include "stdafx.h"
#include "ootypes.h"
#include "fonts.h"

//void BuildFont(HFONT *font)
//{
//
//begin
//
//Base:=glGenLists(224);
//
//Font:=CreateFont(-Size, //высота
//                  Width, //ширина
//                  0, //угол отношения
//                  0, //угол наклона
//                  FW_BOLD, //Широта шрифта
//                  0, //курсив
//                  0, //подчеркивание
//                  0, //перечеркивание
//                  ANSI_CHARSET, //идентификатор небора символов
//                  OUT_TT_PRECIS, //точность вывода
//                  CLIP_DEFAULT_PRECIS, //точность отсечения
//                  ANTIALIASED_QUALITY, //качество вывода
//                  FF_DONTCARE or DEFAULT_PITCH, //семейство и шаг
//                  PChar(Name));
//
//if Font = 0 then begin
//   MessageBox(0, 'Не удается создать шрифт!', 'Ошибка', MB_OK or MB_ICONERROR);
//   Exit;
//  end;
//
//if SelectObject(H_DC, font)=NULL then MessageBox(0, 'Не удается применить шрифт к устройству!', 'Ошибка', MB_OK or MB_ICONERROR);;
//if not wglUseFontBitmaps(H_DC, 32, 224, base) then MessageBox(0, 'Не удается использовать шрифт!', 'Ошибка', MB_OK or MB_ICONERROR);