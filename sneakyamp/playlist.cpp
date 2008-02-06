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

// Playlist functions
#include <windowsx.h>
#include <vector>
#include <string>
#include "playlist.h"

;using namespace std;

extern HWND g_hwndWinamp, g_hwndPlaylist;
// 
vector<string> files;


void GetInfoResponse(char* title)
{
	files.push_back(title);
}

int GetPlaylistSize()
{
	return (int)SendMessage(g_hwndPlaylist, WM_USER, IPC_PE_GETINDEXTOTAL,0);
}

// Get the filename at the given index.
// I use the same functions as GetPlaylistFile,
// so I pull the data out of a vector. Good enough.
string GetPlaylistFile(unsigned int index)
{
	GetInfoCallback infoCallback;
	COPYDATASTRUCT copydata; 
	unsigned int numSongs = GetPlaylistSize();

	if(index >= numSongs)
		return "";

	infoCallback.index = index;
	infoCallback.callback = g_hwndWinamp;
	copydata.dwData = IPC_PE_GETINDEXINFO; 
	copydata.lpData = (void*)&infoCallback;
	copydata.cbData = sizeof(GetInfoCallback); 

	SendMessage(g_hwndPlaylist,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&copydata); 

	// Wait until we have the filename (with a timeout)
	
	for(int i = 0; i < 10 && files.size() == 0; i++)
		Sleep(10);

	if(files.size() != 0)
		return files[0];
	

	return "";
}

vector<string>* GetPlaylistFiles()
{
	
	GetInfoCallback infoCallback;
	COPYDATASTRUCT copydata; 
	//HWND hwndPlaylist = NULL;
	int numSongs = 0;
	//char buffer[100];

	files.clear();

	//hwndPlaylist = FindWindow("Winamp PE",NULL);

	//if(hwndPlaylist == NULL)
	//	return &files;

	numSongs = GetPlaylistSize();//SendMessage(hwndPlaylist, WM_USER, IPC_PE_GETINDEXTOTAL,0);
	//sprintf(buffer,"Num songs: %i", numSongs);
	//MessageBox(hwndPlaylist,buffer,"test",MB_OK);
	if(numSongs == 0)
		return &files;

	infoCallback.callback = plugin.hwndParent;
	copydata.dwData = IPC_PE_GETINDEXINFO; 
	copydata.lpData = (void*)&infoCallback;
	copydata.cbData = sizeof(GetInfoCallback); 

	for(int i = 0; i < numSongs; i++)
	{
		infoCallback.index = i;

		SendMessage(g_hwndPlaylist,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&copydata); 
	}

	for(int i = 0; i < 10 && files.size() != numSongs; i++)
		Sleep(10);
	
	return &files;

}

int GetPlaylistIndex()
{
	//HWND hwndPlaylist = FindWindow("Winamp PE",NULL);

	//if(hwndPlaylist == NULL)
	//	return -1;

	return (int)SendMessage(g_hwndPlaylist,WM_USER,IPC_PE_GETCURINDEX ,0); 
}

void SetPlaylistIndex(int index)
{
	PostMessage(g_hwndWinamp,WM_USER,index, 121);
}

void DeletePlaylistEntry(int index)
{
	SendMessage(g_hwndPlaylist, WM_USER, IPC_PE_DELETEINDEX,index);

}

void InsertSong(string filename, int index)
{
	COPYDATASTRUCT cds;
	FileInfo fileinfo;

	strcpy(fileinfo.file,filename.c_str());
	fileinfo.index = index;

	cds.dwData = IPC_PE_INSERTFILENAME;
	cds.lpData = &fileinfo;
	cds.cbData = sizeof(FileInfo);

	SendMessage(g_hwndPlaylist, WM_COPYDATA, NULL, (LPARAM)&cds);
}

// Shifts a song currently in the playlist so it plays next
// By default, takes only the index of the song to be shifted
// But the filename can also be passed as well for efficiency. 
// I need to determine filename to insert the song in its new position, 
// so if the calling function already knows the filename it saves me a 
// Winamp query
/*
void ShiftSongToNext(int index, const string filename)
{
	COPYDATASTRUCT cds;
	FileInfo fileinfo;
	string tempFilename;

	if(filename == "")
		tempFilename = GetPlaylistFile(index);

	DeletePlaylistEntry(index);

	strcpy(fileinfo.file,tempFilename.c_str());
	tempFilename.index = GetPlaylistIndex() + 1;

	cds.dwData = IPC_PE_INSERTFILENAME;
	cds.lpData = &fileinfo;
	cds.cbData = sizeof(FileInfo);

	SendMessage(g_hwndPlaylist, WM_COPYDATA, NULL, (LPARAM)&cds);
}*/