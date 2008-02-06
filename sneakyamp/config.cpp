/************************************************
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
 *************************************************/

#include "config.h"
#include "stringutil.h"
#include <string>
using namespace std;

// temp debug bit
extern string input;

// Bools to track if the About or Macros window is open,
// so the user doesnt go opening 800 of them
static bool about_open = false,
	        macro_open = false;

BOOL CALLBACK AboutProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if ((uMsg == WM_COMMAND) && (LOWORD(wParam) == IDOK))
	{
		about_open = false;
		EndDialog(hwndDlg,0);
	}
	return false;
}

BOOL CALLBACK MacroProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int i;
	char buffer[MAX_PATH];//FIXME: I dont like this. 

	switch (uMsg)
	{
	case WM_INITDIALOG:
		for (i = 23; i < 33; i++)
		{
			SetDlgItemText(hwndDlg,StringOptions[i].ID, StringOptions[i].Edit.c_str());
		}
		return false;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			macro_open = false;
			EndDialog(hwndDlg,0);
			return false;
		case IDOK:
			for (i = 23; i < 33; i++)
			{
				GetDlgItemText(hwndDlg,StringOptions[i].ID, buffer, MAX_PATH);
				(StringOptions[i].Edit) = buffer;
			}
			macro_open = false;
			EndDialog(hwndDlg,0);
			return false;
		}
		return false;
	}
	return false;
}
			



BOOL CALLBACK ConfigProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char buffer[MAX_PATH]; //FIXME: I dont like this...


	switch (uMsg)
	{
		case WM_INITDIALOG:
			{
				char string[32];
				for (int i = 0; i < 6; i++)
				{
					CheckDlgButton(hwndDlg,IntOptions[i].ID,(IntOptions[i].Edit?BST_CHECKED:BST_UNCHECKED));
				}

				for (i = 0; i < 23; i++)
				{
					SetDlgItemText(hwndDlg,StringOptions[i].ID, StringOptions[i].Edit.c_str());
				}
				for (i = 6; i < 9; i++)
				{
					wsprintf(string,"%d",IntOptions[i].Edit);
					SetDlgItemText(hwndDlg,IntOptions[i].ID, string);
				}

				wsprintf(string,"%d",debug_keycode);
				SetDlgItemText(hwndDlg,IDC_KEY, string);

				SetDlgItemText(hwndDlg,IDC_DEBUG_INPUT,input.c_str());

			}
			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_ABOUT:
					{
						if(!about_open)
						{
							about_open = true;
							DialogBox(plugin.hDllInstance,MAKEINTRESOURCE(IDD_ABOUT),plugin.hwndParent,AboutProc);		
						}
						return false;
					}
				case IDC_MACROS:
					{
						if(!macro_open)
						{
							macro_open = true;
							DialogBox(plugin.hDllInstance,MAKEINTRESOURCE(IDD_MACROS),plugin.hwndParent,MacroProc);		
						}
						return false;
					}
				case IDC_APPLY:
				case IDOK:
					{
						CString temppath1, temppath2;
						char string[30];
						GetCurrentDirectory(MAX_PATH, temppath1.GetBufferSetLength(MAX_PATH));
						temppath1.ReleaseBuffer();
						GetDlgItemText(hwndDlg,IDC_MP3LOCEDIT,temppath2.GetBufferSetLength(MAX_PATH), MAX_PATH);
						temppath2.ReleaseBuffer();
						if (SetCurrentDirectory(temppath2))
						{
							StringOptions[17].Edit = temppath2;
							
							if (IsDlgButtonChecked(hwndDlg,IDC_ENABLECHECK))
							{
								if (IntOptions[0].Edit == 0)
								{
									SetHWnd(plugin.hwndParent);
									if(!SetKeyboardHook())
										IntOptions[0].Edit = 0;
									else
										IntOptions[0].Edit = 1;
								}
								else
									IntOptions[0].Edit = 1;
								
								
							}
							else
							{
								if (IntOptions[0].Edit ==1)
								{
									UnSetKeyboardHook();
								}
								IntOptions[0].Edit = 0;
							}

							if(IntOptions[0].Edit ==1)
								SetDirectoryStructure();

							GetDlgItemText(hwndDlg,IDC_ECEDIT,buffer, MAX_PATH);
							StringOptions[1].Edit = buffer;
							if (IsDlgButtonChecked(hwndDlg, IDC_ECENTERRADIO))
							{
								//end_com = (CHAR) 13;
								IntOptions[3].Edit = 0;
								IntOptions[4].Edit = 1;
							}
							else
							{
								IntOptions[3].Edit = 1;
								IntOptions[4].Edit = 0;
							}
							
							GetDlgItemText(hwndDlg,IDC_BCEDIT,buffer, MAX_PATH);
							StringOptions[0].Edit = buffer;
							if (IsDlgButtonChecked(hwndDlg, IDC_BCENTERRADIO))
							{
								//bgn_com = (CHAR) 13;
								IntOptions[1].Edit = 0;
								IntOptions[2].Edit = 1;
							}
							else
							{
								IntOptions[1].Edit = 1;
								IntOptions[2].Edit = 0;
							}

							if (((IntOptions[4].Edit & IntOptions[2].Edit) == 1) ||
							   (((IntOptions[1].Edit & IntOptions[3].Edit) == 1) && (StringOptions[0].Edit == StringOptions[1].Edit)))
							{
								MessageBox(hwndDlg, "Begin and End command can not be the same", "Error", MB_OK | MB_ICONEXCLAMATION | MB_DEFBUTTON1);
								return FALSE;
							}


							if (IsDlgButtonChecked(hwndDlg, IDC_MP3CHECK))
								IntOptions[5].Edit = 1;
							else
								IntOptions[5].Edit = 0;

							GetDlgItemText(hwndDlg,IDC_MAXFILEEDIT,string, 30);
							IntOptions[7].Edit = atoi(string);
							GetDlgItemText(hwndDlg,IDC_DEFAULTFORREWEDIT,string, 30);
							IntOptions[6].Edit = atoi(string);
							
							for (int i = 2; i < 23; i++)
							{
								GetDlgItemText(hwndDlg,StringOptions[i].ID,buffer, MAX_PATH);
								StringOptions[i].Edit = buffer;
							}
							/*
							GetDlgItemText(hwndDlg,IDC_STOPEDIT,StringOptions[4].Edit.GetBufferSetLength(MAX_PATH), MAX_PATH);
							StringOptions[4].Edit.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_PAUSEEDIT,pause_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							pause_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_PREVIOUSEDIT,prev_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							prev_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_NEXTEDIT,next_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							next_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_REWINDEDIT,rew_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							rew_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_FORWARDEDIT,ff_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							ff_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_VOLUMEEDIT,vol_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							vol_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_BALANCEEDIT,pan_com.GetBufferSetLength(MAX_PATH), MAX_PATH);
							pan_com.ReleaseBuffer();
							GetDlgItemText(hwndDlg,IDC_ADDEDIT,StringOptions[3].Edit.GetBufferSetLength(MAX_PATH), MAX_PATH);
							StringOptions[3].Edit.ReleaseBuffer();
							*/
							GetDlgItemText(hwndDlg,IDC_DIRPREFIXEDIT,buffer, MAX_PATH);
							StringOptions[18].Edit = buffer;
							//FIXME
							// GET HOTKEYS

						}
						else
						{
							MessageBox(hwndDlg, "Invalid MP3 Path", "Error", MB_OK | MB_ICONEXCLAMATION | MB_DEFBUTTON1);
							return FALSE;
						}
						SetCurrentDirectory(temppath1);
					}
				case IDCANCEL:
					if (LOWORD(wParam) != IDC_APPLY) EndDialog(hwndDlg,0);
				return FALSE;
			}
	}
	return FALSE;
}

string GetWinampMediaFiletypes()
{
	char iniFile[MAX_PATH],buffer[MAX_PATH];
	char *pos;
	string result;
	vector<string> filetypes;
	GetWindowModuleFileName(plugin.hwndParent,iniFile,sizeof(iniFile));
	
	pos = strrchr(iniFile,(int)'\\');
	strcpy(pos+1,"winamp.ini");

	GetPrivateProfileString("Winamp", "config_extlist", "", buffer, MAX_PATH, iniFile);
	result = buffer;
	StrTokenize(&filetypes,&result,':');
	result = "";
	for(int i = 0; i < filetypes.size();i++)
	{
		if(!(_stricmp(filetypes[i].c_str(),"M3U") == 0 || _stricmp(filetypes[i].c_str(),"PLS") == 0))
			result += filetypes[i] + ";";
	}
	result = Left(result,result.length()-1);
	return result;
}

void config_read()
{
	char ini_file[MAX_PATH], *p, buffer[MAX_PATH];
	GetModuleFileName(plugin.hDllInstance,ini_file,sizeof(ini_file));
	p=ini_file+lstrlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	lstrcat(ini_file,"plugin.ini");

	first_start = GetPrivateProfileInt(szAppName,"First Start",1,ini_file);
	if (first_start == 1)
	{
		IntOptions[0].Edit = 0;
		first_start = 0;
		for (int i = 0; i < NUM_OF_STRS; i++)
		{
			StringOptions[i].Edit = StringOptions[i].Default;
		}
		for (i = 0; i < NUM_OF_INTS; i++)
		{
			IntOptions[i].Edit = IntOptions[i].Default;
		}
	}
	else
	{
		for (int i = 0;i < NUM_OF_INTS; i++)
		{
			IntOptions[i].Edit = GetPrivateProfileInt(szAppName,IntOptions[i].Description.c_str(),IntOptions[i].Default,ini_file);
		}

		if ((IntOptions[1].Edit + IntOptions[2].Edit) == 0)
		{
			IntOptions[1].Edit = 1;
			IntOptions[2].Edit = 0;
		}
		if ((IntOptions[3].Edit + IntOptions[4].Edit) == 0)
		{
			IntOptions[3].Edit = 1;
			IntOptions[4].Edit = 0;
		}
	
		for (i = 0; i < NUM_OF_STRS; i++)
		{
			GetPrivateProfileString(szAppName, StringOptions[i].Description.c_str(), StringOptions[i].Default.c_str(), buffer, MAX_PATH, ini_file);
			StringOptions[i].Edit = buffer;
		}

		if(StringOptions[STR_MEDIAFILETYPES].Edit == "")
			StringOptions[STR_MEDIAFILETYPES].Edit = GetWinampMediaFiletypes();
	}
}

void config_write()
{
	char ini_file[MAX_PATH], *p, buffer[MAX_PATH];
	//char string[32];
	string test1,test2;
	GetModuleFileName(plugin.hDllInstance,ini_file,sizeof(ini_file));
	p=ini_file+lstrlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	lstrcat(ini_file,"plugin.ini");

	for (int i = 0; i < NUM_OF_STRS; i++)
	{
		test1 = Left(StringOptions[i].Edit,1);
		test2 = Right(StringOptions[i].Edit, 1);
		if (((StringOptions[i].Edit).length() > 1) && (test1 == test2) && ((test1 =="'") || (test1 == "\"")))
		{
			StringOptions[i].Edit = test1 + StringOptions[i].Edit + test1;
		}
		WritePrivateProfileString(szAppName, StringOptions[i].Description.c_str(), StringOptions[i].Edit.c_str(), ini_file);
	}

	// Write integer options
	for (int i = 0; i < NUM_OF_INTS; i++)
	{
		wsprintf(buffer,"%d",IntOptions[i].Edit);
		WritePrivateProfileString(szAppName, IntOptions[i].Description.c_str(), buffer, ini_file);
	}
	
	wsprintf(buffer,"%d",first_start);
	WritePrivateProfileString(szAppName,"First Start",buffer,ini_file);
	

}
