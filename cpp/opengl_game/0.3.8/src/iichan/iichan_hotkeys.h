#ifndef __IICHAN_HOTKEYS_H_
#define __IICHAN_HOTKEYS_H_

// ������� ��� ������������?
const int IICHAN_RESTART = VK_R;
const int IICHAN_SHOWINFO = VK_F5;
const int IICHAN_DISABLE_AI = VK_I;

// ������� ������ ������-�������, � � ����� ��������� ��� ������ �������, ����� ���������� ��������� �������
// �����

map<int, void*> hotkey_map;

void init_hotkey_map(void)
{
	map[VK_R] = 
	