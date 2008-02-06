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


#include "sneakyamp.h"
#include "stdafx.h"
#include "OptionsDlg.h"
#include "playlist.h"
#include "stringutil.h"
#include "basicWinampCommands.h"
#include "newCommands.h"

#include <windowsx.h>

#include <process.h>
#include <time.h>
#include <string>



#include "inputparse.h"

BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

#define BEGIN_COM "winamp"
#define END_COM "\"\""
#define MP3_PATH "d:\\mp3s"
#define MAX_FILES 10;

//DEBUG
int debug_keycode = 0;

// Winamp HWNDs
HWND g_hwndWinamp;
HWND g_hwndPlaylist;


// Vector for storing the directory tokens
// FIXME: Why is this defined here?
vector<string> dir_tokens;

// Used to identify if this is the first time SneakyAmp has run
int first_start = TRUE;

// Used to identify if the keyboard hook has been set or not.
//bool hhook = false;

// Application name
char szAppName[] = "Sneaky Amp";

winampGeneralPurposePlugin plugin =
{
	GPPHDR_VER,
	"",
	init,
	config,
	quit,
};

extern string input;

// FIXME: Is this the best place to define these?
vector<string> g_vFilenames;
vector<string> directories;

stropt StringOptions[NUM_OF_STRS];
intopt IntOptions[NUM_OF_INTS];


BOOL CALLBACK ConfigProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

void config_write();
void config_read();
void FileSearch(const CString);
void SetDirectoryStructure(const CString);

const int MAX_NAME_SIZE = MAX_PATH;


//CString input = "";
//CString searchkey = "";
//CString dirkeylist[24];
//CString keylist[24];




void config()
{
	//HWND temphwnd;/
	//char buffer[255];
	//int ret;
	//COptionsDlg optionsdlg;
	//ret = optionsdlg.DoModal();
	//ret = GetLastError();
	//sprintf(buffer, "%i", ret);
	//MessageBox(hwnd, buffer, "blah", MB_OK);
	DialogBox(plugin.hDllInstance,MAKEINTRESOURCE(IDD_DIALOG1),plugin.hwndParent,ConfigProc);
}

void quit()
{
	directories.clear();
	g_vFilenames.clear();

	//if (hhook != 0)
		UnSetKeyboardHook();
	config_write();
	IntOptions[0].Edit =0;

	UnregisterHotKey(g_hwndWinamp,MEDIA_PLAY_PAUSE);
}

// Returns 0 on bad char (undisplayable usually)
char decode(WPARAM wParam, LPARAM lParam)
{
	int keycode = (INT)lParam;

	char test[2];
	test[1] = '\0';
	WORD test2;
	BYTE ks[256];
	GetKeyboardState(ks);
	int result = ToAscii((UINT)wParam,(lParam&0xff0000)>>16,ks,&test2,0);
	if(result == 1)
	{
		if((test2&0xFF) == ' ')
			MessageBox(NULL,input.c_str(),"feh",MB_OK);
		if((test2&0xFF) > 31)
		{
			//test[0] = test2&0xFF;
			//MessageBox(NULL,test,"feh",MB_OK);
			return test2&0xFF;
		}
		else
			return 0;
	}
	return 0;

	// If alt or ctrl are held down, this isnt a displayable char, so return 0
	if((keycode & 1<<9) || (keycode & 1<<10))
		return 0;

	if (((keycode&255) >= 65) && ((keycode&255) <= 90)) // If a letter was pressed...
			return keycode + 32; // Then the keycode is the ASCII code, return it
							     // (ignoring shift state)
								 // Add 32 to return lower case letters only
	
	
	if(keycode & 1<<8) // Shift held down
	{

		keycode -= 1<<8; // Strip shift bit, to get straight keycode

		
		switch(keycode) // Take care of all the funny keys
		{

			case VK_COMMA:
				return 60; // <
			case VK_PERIOD:
				return 62; // >
			case VK_QUESTION:
				return 63; // ?
			case VK_SEMICOLON:
				return 58; // :
			case VK_QUOTE:
				return 34; // '\'
			case VK_LEFTBRACKET:
				return 123; // {
			case VK_RIGHTBRACKET:
				return 125; // }
			case VK_SLASH:
				return 124; // |
			case VK_EQUALS:
				return 43;  // +
			case VK_MINUS:
				return '_'; // Here I realized returning numbers was a bit odd. :P
			case VK_TILDE:
				return '~';
			case VK_0:
				return ')';
			case VK_1:
				return '!';
			case VK_2:
				return '@';
			case VK_3:
				return '#';
			case VK_4:
				return '$';
			case VK_5:
				return '%';
			case VK_6:
				return '^';
			case VK_7:
				return '&';
			case VK_8:
				return '*';
			case VK_9:
				return '(';
			case VK_SPACE:
				return ' '; // Shift-space == space, I guess
		//	case VK_ENTER: // Should shift-enter == enter? Hmm. Nah.
				//return 13;
				//break;
			default:
				return 0;
		}
		
	}
	else // Shift key not held
	{

		if((keycode >= 48) && (keycode <= 57)) // If a number key was pressed...
			return keycode; // Then the keycode is the ASCII code, return it
		switch(keycode)
		{
			case VK_COMMA:
				return ','; 
			case VK_PERIOD:
				return '.'; 
			case 191: //VK_QUESTION:
				return '/'; 
			case VK_SEMICOLON:
				return ';'; 
			case VK_QUOTE:
				return '"'; 
			case VK_LEFTBRACKET:
				return '[';
			case VK_RIGHTBRACKET:
				return ']';
			case VK_SLASH:
				return '\\';
			case VK_EQUALS:
				return '='; 
			case VK_MINUS:
				return '-'; // Here I realized return numbers was a bit odd. :P
			case VK_TILDE:
				return '`';
			case VK_SPACE:
				return ' ';
			case 13:  //VK_ENTER: 
				return 13;
			case 8:   // BAckspace
				return 8;
			default:
				return 0;
		}

	}
}

// takes the keyboard code from the keyboard hook, and sees if it matches
// a hotkey definition
//
// 3/15/2004 
// This function will start taking hotkey id numbers as well. The 2 shouldnt
// overlap, so the 1 function will work fine for both sets of numbers,
// at least until I switch to RegisterHotkey completely
bool hotkeys(LPARAM keycode)
{
	//char buffer[100];
	//sprintf(buffer,"%i",keycode);
	//MessageBox(NULL,buffer,"woo",MB_OK);

	switch(keycode)
	{
	case 1370: // ctrl-shift-Z (PREV)
		HitPrevButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40044, 0);
		return true;
	case 1368: // ctrl-shift-X (PLAY)
		HitPlayButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);
		return true;
	case 1347: // ctrl-shift-C (PAUSE)
		HitPauseButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40046, 0);
		return true;
	case 1366: // ctrl-shift-V (STOP)
		HitStopButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40047, 0);
		return true;
	case 1346: // ctrl-shift-B (NEXT)
		HitNextButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40048, 0);
		return true;
	case 1361: // ctrl-shift-V (STOP)
		ResetKeyboardHook();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40047, 0);
		return true;
	
	// Its not easy to find the current volume, so until
    // I figure that out, I cant really arbitrarily move the volume
	// up or down, only set it at specific numbers. Maybe I'll
	// have SneakyAmp retain its own made up volume number. It'd be
	// funky, but it'd kinda work. Kinda.
	// EDIT 3/1/2004
	// It turns out that its easy to get the current volume, but
	// it was undocumented. Theres also commands to turn the volume up and down,
	// but I dont know if they work in old versions of Winamp
	case 1469: // ctrl-shift-- (VOL DOWN)
		VolumeDown();
		return true;
	case 1467: // ctrl-shift-+ (VOL UP)
		VolumeUp();
		return true;

	case 1329: // ctrl-shift-1 (Hotkey 1)
		input = StringOptions[23].Edit;
		inputParse(0);
		return true;
	case 1330: // ctrl-shift-2 (Hotkey 2)
		input = StringOptions[24].Edit;
		inputParse(0);
		return true;
	case 1331: // ctrl-shift-3 (Hotkey 3)
		input = StringOptions[25].Edit;
		inputParse(0);
		return true;
	case 1332: // ctrl-shift-4 (Hotkey 4)
		input = StringOptions[26].Edit;
		inputParse(0);
		return true;
	case 1333: // ctrl-shift-5 (Hotkey 5)
		input = StringOptions[27].Edit;
		inputParse(0);
		return true;
	case 1334: // ctrl-shift-6 (Hotkey 6)
		input = StringOptions[28].Edit;
		inputParse(0);
		return true;
	case 1335: // ctrl-shift-6 (Hotkey 7)
		input = StringOptions[29].Edit;
		inputParse(0);
		return true;
	case 1336: // ctrl-shift-6 (Hotkey 8)
		input = StringOptions[30].Edit;
		inputParse(0);
		return true;
	case 1337: // ctrl-shift-6 (Hotkey 9)
		input = StringOptions[31].Edit;
		inputParse(0);
		return true;
	case 1328: // ctrl-shift-6 (Hotkey 0)
		input = StringOptions[32].Edit;
		inputParse(0);
		return true;

	// 3/15/2004
	case MEDIA_PLAY_PAUSE: // Play/Pause multimedia key
		HitPlayPause();
		return true;

	default:
		return false;
	}

	return false;
}

WNDPROC lpWndProcOld;
LRESULT CALLBACK WndProc(HWND hwndWinamp, UINT message, WPARAM wParam, LPARAM lParam)
{


	char Char; // Bah, well what would YOU call it? :)

	// Handle playlist responses...
	if(message == WM_COPYDATA && ((COPYDATASTRUCT*)lParam)->dwData == IPC_PE_GETINDEXINFORESULT)
	{
		GetInfoResponse((char*)((COPYDATASTRUCT*)lParam)->lpData);
		//COPYDATASTRUCT temp = *((COPYDATASTRUCT*)lParam);
		//char buffer[2000];
		//sprintf(buffer,"Copydata.dwData : %i, lpData: %s", temp.dwData, temp.lpData);
		//MessageBox(plugin.hwndParent,buffer,"test",MB_OK);

	}
	else if (message == WM_USER + 999)
	{
		
		
		//if(LOWORD(wParam) == WM_USER + 999)
		//{

			// Little test, to make sure hook is working
			// code gets displayed on options page
			debug_keycode = (int)wParam;
			
			//FILE *file;
			//file = fopen("c:\\sneakytest.txt","a");
			//fprintf(file,"%i\n",wParam);
			//fclose(file);
			if(!hotkeys(wParam))
			{
				Char = lParam;//decode(wParam,lParam);
				//string test;
				//test = Char;
				//MessageBox(NULL,test.c_str(),"Feh",MB_OK);
				if(Char == 8 || Char > 31) // Ignore all non-display characters except backspace
				{		
					inputParse(Char);
				}
			}
			//else
			//	MessageBox(NULL,"Caught a hotkey","woo",MB_OK);
		
		//}
	}
	else if(message == WM_HOTKEY)
	{
		hotkeys(wParam);
	}
	return CallWindowProc(lpWndProcOld,hwndWinamp,message,wParam,lParam);
}

int init()
{
	srand( (unsigned)time( NULL ) );

	//MessageBox(NULL, (Left("abcdefg",3)).c_str(), "test", MB_OK);

	// Set global HWNDs
	// 
	g_hwndWinamp = plugin.hwndParent;
	// Use FindWindow method first, just in case we're runnin an old version
	// I could test for version, but its not that big of a deal
	// maybe someday
	g_hwndPlaylist = FindWindow("Winamp PE",NULL);
	if(g_hwndPlaylist == NULL)
		g_hwndPlaylist = GetPlaylistHWND();


	StringOptions[0].Default = BEGIN_COM;
	StringOptions[0].Description = "Begin Command";
	StringOptions[0].ID = IDC_BCEDIT;
	StringOptions[1].Default = END_COM;
	StringOptions[1].Description = "End Command";
	StringOptions[1].ID = IDC_ECEDIT;
	StringOptions[2].Default = "play";
	StringOptions[2].Description = "Play Command";
	StringOptions[2].ID = IDC_PLAYEDIT;
	StringOptions[3].Default = "add";
	StringOptions[3].Description = "Add Command";
	StringOptions[3].ID = IDC_ADDEDIT;
	StringOptions[4].Default = "stop";
	StringOptions[4].Description = "Stop Command";
	StringOptions[4].ID = IDC_STOPEDIT;
	StringOptions[5].Default = "pause";
	StringOptions[5].Description = "Pause Command";
	StringOptions[5].ID = IDC_PAUSEEDIT;
	StringOptions[6].Default = "forward";
	StringOptions[6].Description = "Forward Command";
	StringOptions[6].ID = IDC_FORWARDEDIT;
	StringOptions[7].Default = "prev";
	StringOptions[7].Description = "Previous Command";
	StringOptions[7].ID = IDC_PREVIOUSEDIT;
	StringOptions[8].Default = "next";
	StringOptions[8].Description = "Next Command";
	StringOptions[8].ID = IDC_NEXTEDIT;
	StringOptions[9].Default = "volume";
	StringOptions[9].Description = "Volume Command";
	StringOptions[9].ID = IDC_VOLUMEEDIT;
	StringOptions[10].Default = "balance";
	StringOptions[10].Description = "Balance Command";
	StringOptions[10].ID = IDC_BALANCEEDIT;
	StringOptions[11].Default = "rewind";
	StringOptions[11].Description = "Rewind Command";
	StringOptions[11].ID = IDC_REWINDEDIT;
	StringOptions[12].Default = "shuffle";
	StringOptions[12].Description = "Shuffle Command";
	StringOptions[12].ID = IDC_SHUFFLEEDIT;
	StringOptions[13].Default = "repeat";
	StringOptions[13].Description = "Repeat Command";
	StringOptions[13].ID = IDC_REPEATEDIT;
	StringOptions[14].Default = "on";
	StringOptions[14].Description = "On Command";
	StringOptions[14].ID = IDC_ONEDIT;
	StringOptions[15].Default = "off";
	StringOptions[15].Description = "Off Command";
	StringOptions[15].ID = IDC_OFFEDIT;
	StringOptions[16].Default = "random";
	StringOptions[16].Description = "Random Command";
	StringOptions[16].ID = IDC_RANDOMEDIT;
	StringOptions[17].Default = "c:\\";// = MP3_PATH;
	StringOptions[17].Description = "MP3 Path";
	StringOptions[17].ID = IDC_MP3LOCEDIT;
	StringOptions[18].Default = "-";
	StringOptions[18].Description = "Dir Prefix";
	StringOptions[18].ID = IDC_DIRPREFIXEDIT;

	// New Strings for new mp3 directories
	StringOptions[19].Default = "";
	StringOptions[19].Description = "Dir2";
	StringOptions[19].ID = IDC_MP3LOCEDIT2;
	StringOptions[20].Default = "";
	StringOptions[20].Description = "Dir3";
	StringOptions[20].ID = IDC_MP3LOCEDIT3;
	StringOptions[21].Default = "";
	StringOptions[21].Description = "Dir4";
	StringOptions[21].ID = IDC_MP3LOCEDIT4;

	// New command for playing only playlists
	StringOptions[22].Default = "list";
	StringOptions[22].Description = "Play list command";
	StringOptions[22].ID = IDC_LISTEDIT;

	// New hotkey strings
	StringOptions[23].Default = "play * rand 5";
	StringOptions[23].Description = "Hotkey 1";
	StringOptions[23].ID = IDC_ACTION1;
	StringOptions[24].Default = "";
	StringOptions[24].Description = "Hotkey 2";
	StringOptions[24].ID = IDC_ACTION2;
	StringOptions[25].Default = "";
	StringOptions[25].Description = "Hotkey 3";
	StringOptions[25].ID = IDC_ACTION3;
	StringOptions[26].Default = "";
	StringOptions[26].Description = "Hotkey 4";
	StringOptions[26].ID = IDC_ACTION4;
	StringOptions[27].Default = "";
	StringOptions[27].Description = "Hotkey 5";
	StringOptions[27].ID = IDC_ACTION5;
	StringOptions[28].Default = "";
	StringOptions[28].Description = "Hotkey 6";
	StringOptions[28].ID = IDC_ACTION6;
	StringOptions[29].Default = "";
	StringOptions[29].Description = "Hotkey 7";
	StringOptions[29].ID = IDC_ACTION7;
	StringOptions[30].Default = "";
	StringOptions[30].Description = "Hotkey 8";
	StringOptions[30].ID = IDC_ACTION8;
	StringOptions[31].Default = "";
	StringOptions[31].Description = "Hotkey 9";
	StringOptions[31].ID = IDC_ACTION9;
	StringOptions[32].Default = "";
	StringOptions[32].Description = "Hotkey 0";
	StringOptions[32].ID = IDC_ACTION0;

	StringOptions[33].Default = "cd";
	StringOptions[33].Description = "CD command";
	StringOptions[33].ID = NULL;
	StringOptions[34].Default = "jump";
	StringOptions[34].Description = "Jump command";
	StringOptions[34].ID = NULL;
	StringOptions[35].Default = "shift";
	StringOptions[35].Description = "Shift command";
	StringOptions[35].ID = NULL;
	StringOptions[36].Default = "addnext";
	StringOptions[36].Description = "Add Next command";
	StringOptions[36].ID = NULL;

	// New command for adding a list (why wasnt this already implemented? Oh well)
	StringOptions[37].Default = "addlist";
	StringOptions[37].Description = "Add List command";
	StringOptions[37].ID = NULL;

	// New command for adding a list (why wasnt this already implemented? Oh well)
	StringOptions[38].Default = "jumptonum";
	StringOptions[38].Description = "Jump To Num command";
	StringOptions[38].ID = NULL;

	// New filetype settings, for filtering search results(3/10/2004)
	StringOptions[39].Default = "delete";
	StringOptions[39].Description = "Delete File command";
	StringOptions[39].ID = NULL;

	// New filetype settings, for filtering search results(3/10/2004)
	StringOptions[40].Default = "";
	StringOptions[40].Description = "Media Filetypes";
	StringOptions[40].ID = NULL;
	StringOptions[41].Default = "M3U;PLS";
	StringOptions[41].Description = "List Filetypes";
	StringOptions[41].ID = NULL;

	// 3/16/2004
	StringOptions[42].Default = "playpause";
	StringOptions[42].Description = "Play/Pause Command";
	StringOptions[42].ID = NULL;

	// 4/24/2004
	StringOptions[43].Default = "playloc";
	StringOptions[43].Description = "Play Location";
	StringOptions[43].ID = NULL;

	// 4/25/2004
	StringOptions[44].Default = "addloc";
	StringOptions[44].Description = "Add Location";
	StringOptions[44].ID = NULL;




	
	IntOptions[0].Default = 0;
	IntOptions[0].Description = "Enabled";
	IntOptions[0].ID = IDC_ENABLECHECK;
	IntOptions[1].Default = 1;
	IntOptions[1].Description = "BCEdit";
	IntOptions[1].ID = IDC_BCEDITRADIO;
	IntOptions[2].Default = 0;
	IntOptions[2].Description = "BCEnter";
	IntOptions[2].ID = IDC_BCENTERRADIO;
	IntOptions[3].Default = 1;
	IntOptions[3].Description = "ECEdit";
	IntOptions[3].ID = IDC_ECEDITRADIO;
	IntOptions[4].Default = 0;
	IntOptions[4].Description = "ECEnter";
	IntOptions[4].ID = IDC_ECENTERRADIO;
	IntOptions[5].Default = 1;
	IntOptions[5].Description = "Assume MP3";
	IntOptions[5].ID = IDC_MP3CHECK;
	IntOptions[6].Default = 10;
	IntOptions[6].Description = "Default Offset";
	IntOptions[6].ID = IDC_DEFAULTFORREWEDIT;
	IntOptions[7].Default = 50;
	IntOptions[7].Description = "Max Files";
	IntOptions[7].ID = IDC_MAXFILEEDIT;

	/*IntOptions[8].Default = 65;
	IntOptions[8].Description = "Play HK";
	IntOptions[8].ID = IDC_PLAYHK;
	IntOptions[9].Default = 66;
	IntOptions[9].Description = "Stop HK";
	IntOptions[9].ID = IDC_STOPHK;
	IntOptions[10].Default = 67;
	IntOptions[10].Description = "Next HK";
	IntOptions[10].ID = IDC_NEXTHK;
	IntOptions[11].Default = 68;
	IntOptions[11].Description = "Prev HK";
	IntOptions[11].ID = IDC_PREVHK;
	IntOptions[12].Default = 69;
	IntOptions[12].Description = "Pause HK";
	IntOptions[12].ID = IDC_PAUSEHK;
*/
	{
		static char c[512];
		char filename[512],*p;
		GetModuleFileName(plugin.hDllInstance,filename,sizeof(filename));
		p = filename+lstrlen(filename);
		while (p >= filename && *p != '\\') p--;
		wsprintf((plugin.description=c),"%s Plug-In v2.0 (%s)",szAppName,p+1);
	}

	config_read();

	
	//lpWndProcOld = (WNDPROC) GetWindowLong(plugin.hwndParent, GWL_WNDPROC);
	lpWndProcOld = (WNDPROC) SetWindowLong(plugin.hwndParent, GWL_WNDPROC,(LONG)WndProc);
	
	SetHWnd(plugin.hwndParent);
	if (IntOptions[0].Edit == 1)
		if(!SetKeyboardHook())
			IntOptions[0].Edit = 0; // Disable if the hook fails
	
	// If a directory is defined, and we're enabled, go looking at directories
	if (StringOptions[17].Edit != "" && IntOptions[0].Edit == 1) 
		SetDirectoryStructure();

	RegisterHotKey(g_hwndWinamp,MEDIA_PLAY_PAUSE,0,VK_MEDIA_PLAY_PAUSE);

	return 0;
}









extern "C" __declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin()
{
	return &plugin;
}

/*__declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin()
{
	return &plugin;
}*/

