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

#pragma once

#include <afx.h>
#include <string>

using namespace std;

// Winamp HWNDs
extern HWND g_hwndWinamp;
extern HWND g_hwndPlaylist;

void HitPlayButton();
void HitStopButton();
void ClearPlaylist();
void AddSongToPlaylist(string song);
void HitNextButton();
void HitPrevButton();
void HitPauseButton();
void ToggleRandom();
void TurnRandomOn();
void TurnRandomOff();
void ToggleRepeat();
void TurnRepeatOn();
void TurnRepeatOff();
void PlayCD(int driveNum = 1);
void SetVolume(int volume);
void SetBalance(int balance);
int GetSongPosition();
void SetSongPosition(int position);
char* GetSongFile(int pos);
int GetCurrentSongIndex();


// Added 3/1/2004
bool IsPlaying();
int GetVolume();

// 3/8/2004
HWND GetPlaylistHWND();

// 3/15/2004
void HitPlayPause();
