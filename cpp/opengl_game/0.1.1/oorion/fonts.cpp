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
//Font:=CreateFont(-Size, //������
//                  Width, //������
//                  0, //���� ���������
//                  0, //���� �������
//                  FW_BOLD, //������ ������
//                  0, //������
//                  0, //�������������
//                  0, //��������������
//                  ANSI_CHARSET, //������������� ������ ��������
//                  OUT_TT_PRECIS, //�������� ������
//                  CLIP_DEFAULT_PRECIS, //�������� ���������
//                  ANTIALIASED_QUALITY, //�������� ������
//                  FF_DONTCARE or DEFAULT_PITCH, //��������� � ���
//                  PChar(Name));
//
//if Font = 0 then begin
//   MessageBox(0, '�� ������� ������� �����!', '������', MB_OK or MB_ICONERROR);
//   Exit;
//  end;
//
//if SelectObject(H_DC, font)=NULL then MessageBox(0, '�� ������� ��������� ����� � ����������!', '������', MB_OK or MB_ICONERROR);;
//if not wglUseFontBitmaps(H_DC, 32, 224, base) then MessageBox(0, '�� ������� ������������ �����!', '������', MB_OK or MB_ICONERROR);