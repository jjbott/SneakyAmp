/****************************************
 *
 * khk.h
 * SneakyAmp's keyboard hook DLL
 *
 * Written by Joe Bott - Copyright 2000-2006
 *
 * Released under the LGPL on Sep 23, 2006
 * http://www.gnu.org/copyleft/lesser.html
 *
 * In a nutshell, do what you want with the source,
 * but any modifications must be open source under 
 * the LGPL as well.
 *
 ****************************************/


#ifndef _KBDLL2_H
#define _KBDLL2_H

#include<string>
using namespace std;

#define KEY_UP 2147483648
/*
// defines for my vk codes
#define VK_COMMA 188
#define VK_PERIOD 190
#define VK_QUESTION 191
#define VK_SEMICOLON 186
#define VK_QUOTE 222
#define VK_LEFTBRACKET 219
#define VK_RIGHTBRACKET 221
#define VK_SLASH 220
#define VK_EQUALS 187
#define VK_MINUS 189
#define VK_TILDE 192
#define VK_ENTER 13
#define VK_ALT 18

#define VK_1 49
#define VK_2 50
#define VK_3 51
#define VK_4 52
#define VK_5 53
#define VK_6 54
#define VK_7 55
#define VK_8 56
#define VK_9 57
#define VK_0 48
*/

//functions to export
extern "C" _declspec(dllexport) bool SetKeyboardHook();
extern "C" _declspec(dllexport) bool UnSetKeyboardHook();
extern "C" _declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" _declspec(dllexport) bool SetHWnd(HWND);
#endif

