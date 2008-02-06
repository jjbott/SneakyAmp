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

// Playlist functions
#include <windowsx.h>
#include "sneakyamp.h"

#define IPC_PE_GETCURINDEX        100 
#define IPC_PE_GETINDEXTOTAL      101 
#define IPC_PE_GETINDEXINFO       102 
#define IPC_PE_GETINDEXINFORESULT 103 
#define IPC_PE_DELETEINDEX        104 
#define IPC_PE_SWAPINDEX          105 
#define IPC_PE_INSERTFILENAME     106 
#define IPC_PE_GETDIRTY                              107 
#define IPC_PE_SETCLEAN                              108 

extern winampGeneralPurposePlugin plugin;

struct GetInfoCallback 
{ 
	HWND callback; 
    int index; 
}; 

struct FileInfo 
{ 
	char file[MAX_PATH]; 
    int index; 
}; 

int GetPlaylistSize();
void GetInfoResponse(char* title);
vector<string>* GetPlaylistFiles();
int GetPlaylistIndex();
void SetPlaylistIndex(int);
void DeletePlaylistEntry(int);
void InsertSong(string filename, int index);
void ShiftSongToNext(int index, string filename = "");