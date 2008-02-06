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

#include "basicWinampCommands.h"
#include <afx.h>
#include <string>

using namespace std;

void HitPlayButton()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40045, 0);
}

void HitStopButton()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40047, 0);
}

void HitNextButton()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40048, 0);
}

void HitPrevButton()
{
    PostMessage(g_hwndWinamp,WM_COMMAND,40044, 0);
}

void HitPauseButton()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40046, 0);
}

void ToggleRandom()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40023, 0);
}

void TurnRandomOn()
{
	if(SendMessage(g_hwndWinamp,WM_USER,0,250) == 0)
		ToggleRandom();
}

void TurnRandomOff()
{
	if(SendMessage(g_hwndWinamp,WM_USER,0,250) == 1)
		ToggleRandom();
}

void ToggleRepeat()
{
	PostMessage(g_hwndWinamp,WM_COMMAND,40022, 0);
}

void TurnRepeatOn()
{
	if(SendMessage(g_hwndWinamp,WM_USER,0,251) == 0)
		ToggleRepeat();
}

void TurnRepeatOff()
{
	if(SendMessage(g_hwndWinamp,WM_USER,0,251) == 1)
		ToggleRepeat();
}

void ClearPlaylist()
{
	SendMessage(g_hwndWinamp,WM_USER, 0, 101);
}

void PlayCD(int driveNum)
{
	if(driveNum >= 1 && driveNum <= 8) // Limit picked arbitrarily
		PostMessage(g_hwndWinamp,WM_COMMAND,40322 + driveNum, 0);
}

void SetVolume(int volume)
{
	PostMessage(g_hwndWinamp,WM_USER, volume, 122);
}

void SetBalance(int balance)
{
	PostMessage(g_hwndWinamp,WM_USER, balance, 123);
}

int GetSongPosition()
{
	return (int)SendMessage(g_hwndWinamp,WM_USER,0,105);
}

void SetSongPosition(int position)
{
	PostMessage(g_hwndWinamp,WM_USER,position,106);
}

void AddSongToPlaylist(string song)
{
	COPYDATASTRUCT cds;
	cds.dwData = 100;
	cds.cbData = (DWORD)song.length()+1;
	cds.lpData = (void*)song.c_str();

	SendMessage(g_hwndWinamp,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
}

// Added 3/1/2004
// Determines if a song is playing
bool IsPlaying()
{
	int res = (int)SendMessage(g_hwndWinamp,WM_USER,0,104);
	if(res == 1 || res == 3)
		// Playing or paused. I decided paused is closer to playing than stopped
		return true;
	else
		return false;
}

// Added 3/1/2004
// Gets the current volume level
int GetVolume()
{
	// Send a "Set Volume" request with -666 for the volume level,
	// which should return the current volume level
	return SendMessage(g_hwndWinamp,WM_USER,-666,122);
}

// Gets the indicated song's path and filename
char* GetSongFile(int pos)
{
	return (char*)SendMessage(g_hwndWinamp,WM_USER,pos,211);
}

int GetCurrentSongIndex()
{
	return SendMessage(g_hwndWinamp,WM_USER,0,125);
}

HWND GetPlaylistHWND()
{
	return (HWND)SendMessage(g_hwndWinamp,WM_USER,1,260);
}

// 3/15/2004
// A combination of hitting play and hitting pause, used
// for multimedia keyboards with a PLay/Pause button
void HitPlayPause()
{
	if(IsPlaying())
		HitPauseButton();
	else
		HitPlayButton();

}