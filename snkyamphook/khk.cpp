/****************************************
 *
 * khk.cpp
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

#include "stdafx.h"
#include "khk.h"
#include <string>

#pragma comment(linker, "/SECTION:.SHARED,RWS")
#pragma data_seg(".SHARED")
   static HHOOK g_hKeyboardHook=NULL;
   static HWND hwnd = NULL;
   
   static bool shiftstate;
   static bool altstate;
   static bool ctrlstate;
#pragma data_seg()
static HANDLE HookDll=NULL;

WPARAM x = WM_USER + 999;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HookDll = hinstDLL;
	return TRUE;
}

_declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//char buffer[255];
	unsigned int keystate; // wParam + shift/alt/ctrl key states

	if (nCode < 0)
		return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);
	if (nCode == HC_ACTION)
	{
		if(wParam == PM_REMOVE)
		{
			MSG *msg = (MSG*)lParam;

			// Filter everything except WM_KEYDOWN for now
			if(msg->message != WM_KEYDOWN)
				return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);

			// Filter out actions on shift, alt, and control
			if(msg->wParam == VK_SHIFT || msg->wParam == VK_CONTROL || msg->wParam == VK_MENU)
				return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);
		
			// Set shift alt and control key states. 
			if((GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0x8000)
				shiftstate = true;
			else
				shiftstate = false;
			if((GetAsyncKeyState(VK_MENU) & 0x8000) == 0x8000)
				altstate = true;
			else
				altstate = false;
			if((GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0x8000)
				ctrlstate = true;
			else
				ctrlstate = false;
		//sprintf(buffer,"%i %x", wParam, lParam);
		//MessageBox(hwnd, buffer, "blah", MB_OK);
		/*
		if (wParam == VK_SHIFT)
		{
			
			if(lParam&KEY_UP)
			{
				shiftstate = false;
			} 
			else
			{
				shiftstate = true;
			}
		}
		if (wParam == VK_ALT)
		{
			
			if(lParam&KEY_UP)
			{
				altstate = false;
			} 
			else
			{
				altstate = true;
			}
		}
		if (wParam == VK_CONTROL)
		{
			
			if(lParam&KEY_UP)
			{
				ctrlstate = false;
			} 
			else
			{
				ctrlstate = true;
			}
		}
		*/

			// Build keystate, used for hotkeys
			keystate = msg->wParam;
			if(shiftstate)
				keystate += 1<<8;
			if(altstate)
				keystate += 1<<9;
			if(ctrlstate)
				keystate += 1<<10;

			//if (!(lParam&KEY_UP))
			//{
				

				BYTE ks[256];
				WORD ascii = 0;
				
				if(!(ctrlstate || altstate))
				{
					GetKeyboardState(ks);
					//ks[VK_SHIFT]= GetAsyncKeyState(VK_SHIFT) >> 8;
					ToAscii((UINT)msg->wParam,(msg->lParam&0xff0000)>>16,ks,&ascii,0);
					ascii &= 0xff;
				}
				//if(ascii == 8 || ascii > 31) // hotkeys arent sent if this is active
					PostMessage(hwnd,WM_USER+999,keystate,ascii);
			//}
		}

		return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);
	}
	
	return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);


}




_declspec(dllexport) bool SetKeyboardHook()
{   
	//g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, (HINSTANCE)HookDll, NULL);   
	g_hKeyboardHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)KeyboardProc, (HINSTANCE)HookDll, NULL);   
	
	if(g_hKeyboardHook == NULL)   
	{   
		char buffer[100];
		sprintf(buffer, "Keyboard hook failed: Error %i", GetLastError());
		MessageBox(NULL, buffer, "SneakyAmp Error", NULL); 
		return false;
	}   
	return true;
}  

_declspec(dllexport) bool UnSetKeyboardHook()
{   

	if(g_hKeyboardHook != NULL)   
	{   
		if(UnhookWindowsHookEx(g_hKeyboardHook) == 0)
		{
			char buffer[100];
			sprintf(buffer, "Keyboard hook unset failed: Error %i", GetLastError());
			MessageBox(NULL, buffer, "SneakyAmp Error", NULL); 
			return false;
		}
		else
		{
			g_hKeyboardHook = NULL;
			return true;
		}
	}  
	return false;
	
}


_declspec(dllexport) bool SetHWnd(HWND x)
{
	hwnd = x;
	return true;
}