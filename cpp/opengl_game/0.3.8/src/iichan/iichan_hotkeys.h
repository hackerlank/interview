#ifndef __IICHAN_HOTKEYS_H_
#define __IICHAN_HOTKEYS_H_

// Сделать это неконстантым?
const int IICHAN_RESTART = VK_R;
const int IICHAN_SHOWINFO = VK_F5;
const int IICHAN_DISABLE_AI = VK_I;

// создать массив кнопка-событие, и в цикле прогонять его каждое нажатие, чтобы определять системное событие
// домой

map<int, void*> hotkey_map;

void init_hotkey_map(void)
{
	map[VK_R] = 
	