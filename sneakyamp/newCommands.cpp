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

//#include <windows.h>
#include "sneakyamp.h"
#include "stdafx.h"
#include "playlist.h"
#include <vector>
#include <string>
#include <algorithm>
#include "stringutil.h"
#include "basicWinampCommands.h"
#include "commands.h"
#include "newCommands.h"
#include<Shlwapi.h>
using namespace std;

#pragma warning( disable: 4786)

struct fileStruct // I cant think of a good name for this...
{				  // holds the full file path, plus just the filename
	string file;		// Need to sort based on filename, but
	string filename;	// search by full path
	int originalIndex;
};

bool FileStructLess(fileStruct file1, fileStruct file2)
{
	return file1.filename < file2.filename;
}

// Utility function, adds the files we found to Winamp
void InsertSongsUtility(vector<string> *files, int index, int numRand)
{
	int rnd;

	if(numRand != 0)
	{
		for(int i = 0; i < numRand && !files->empty(); i++)
		{
			rnd = rand()%(int)files->size();
			InsertSong((*files)[rnd], index);
			//AddSongToPlaylist((*files)[rnd]);
			files->erase(files->begin() + rnd);
		}
	}
	else
	{
		for(int i = 0; i < (int)files->size(); i++)
		{
			InsertSong((*files)[i], index);
		}
	}
}

string StripPath(string file)
{
	//MessageBox(NULL, "Strippin something", "test", MB_OK);
	int beginPos = 0,temp;
	//char buffer[1000];
	temp = (int)file.find_last_of('\\');
	if(beginPos < temp)
		beginPos = temp +1;
	temp = (int)file.find_last_of('/');
	if(beginPos < temp)
		beginPos = temp +1;
	
	return file.substr(beginPos, file.length()-4-beginPos);
}

void JumpToFile(vector<string> *tokens)
{
	vector<string> *files;
	//char buffer[10000];
	//string cstemp; // Cuz I'm lazy, and I want MakeLower
	vector<fileStruct> filelist; // Gah, another bad name
	fileStruct tempStruct;
	int findPos = 0, temp, jumpToIndex = -1, currentIndex;

	if(tokens->size() <= 1)
		return;

	//MessageBox(NULL, (*tokens)[1], "test", MB_OK);

	files = GetPlaylistFiles();
	for(int i = 0; i < (int)files->size(); i++)
	{
		//cstemp = (*files)[i].c_str();
		//cstemp.MakeLower();
		tempStruct.file = MakeLower((*files)[i]);
		tempStruct.filename = StripPath(tempStruct.file);
		tempStruct.originalIndex = i;
		filelist.push_back(tempStruct);
	}

	sort(filelist.begin(), filelist.end(), FileStructLess);

	
	//for(i = 0; i< filelist.size(); i++)
	//	MessageBox(NULL, filelist[i].filename.c_str(), "test", MB_OK);

	for(int i = 0; i < (int)filelist.size(); i++)
	{
		findPos = 0;
		for(int j = 1; j < (int)tokens->size(); j++)
		{
			if((temp = (int)filelist[i].file.find((*tokens)[j], findPos)) != -1)
				findPos = temp+1;
			else
			{
				findPos = -1;
				break;
			}
		}
		if(findPos >= 0)
		{
			jumpToIndex = i;
			break;
		}
	}

	//MessageBox(NULL, filelist[i].file.c_str(), "test", MB_OK);

	if(jumpToIndex >= 0)
		jumpToIndex = filelist[jumpToIndex].originalIndex;
	else
	{
		//MessageBox(NULL, "Nothing Found", "test", MB_OK);
		return; // nothing found
	}

	
	currentIndex = GetPlaylistIndex();
	
	//sprintf(buffer, "%s Currrent: %i, jumpTo: %i", (*tokens)[1].c_str(), currentIndex, jumpToIndex);
	//MessageBox(NULL, buffer, "test", MB_OK);
	SetPlaylistIndex(jumpToIndex);
	//PostMessage(plugin.hwndParent,WM_USER,jumpToIndex, 121);
	HitPlayButton();
	//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);
	/*
	if(currentIndex < jumpToIndex)
	{
		for(int i = 0; i < jumpToIndex-currentIndex; i++)
		{
			//MessageBox(NULL, "Hitting buttons...", "test", MB_OK);
			PostMessage(plugin.hwndParent,WM_COMMAND,40048, 0);
			//Sleep(50);
		}
	}
	else
	{
		for(int i = 0; i < currentIndex-jumpToIndex; i++)
		{
			//MessageBox(NULL, "Hitting buttons...", "test", MB_OK);
			PostMessage(plugin.hwndParent,WM_COMMAND,40044, 0);
			//Sleep(50);
		}
	}
	*/

}

// Shift 1 song curently in the playlist to play next 
// Return true if anything was shifted, else return false
bool ShiftSongsToNext(const vector<string> *tokens)
{
	vector<string> *files; // All of the songs in the playlist
	vector<string> matchedFiles; // Files we want to insert to play next
	vector<string> searchWords; // To pass to string compare util, same as tokens but strips the first one

	int currentIndex, i;
	int numDeleted = 0;

	if(tokens->size() <= 1)
		return false;

	for(i = 1; i < (int)tokens->size(); i++)
		searchWords.push_back((*tokens)[i]);

	files = GetPlaylistFiles();
	currentIndex = GetPlaylistIndex();

	for(i = 0; i < (int)files->size(); i++)
		if(i != currentIndex && StringMatch(&((*files)[i]), &searchWords))
		{
			matchedFiles.push_back((*files)[i]);

			DeletePlaylistEntry(i-numDeleted);

			numDeleted++;
		}
			//ShiftSongToNext(int index, const string filename)

	InsertSongsUtility(&matchedFiles, GetPlaylistIndex()+1);

	if(matchedFiles.size() > 0)
		return true;

	return false;

	/*
	if(i != files->size())
	{
		//MessageBox(NULL, "Shifting!", "test", MB_OK);
		DeletePlaylistEntry(i);

		// Deleting may have changed the current index, so get it again
		currentIndex = GetPlaylistIndex();

		InsertSong((*files)[i], currentIndex + 1);
	}*/
}

// Adds songs to the playlist, to play next 
void AddSongsToNext(vector<string> *tokens, int numRand)
{
	vector<string> *files;

	files = Search(tokens);
	if(files->size() > 0)
	{
		InsertSongsUtility(files, GetPlaylistIndex()+1, numRand);
	}

}


// Added 12/04/2003
void ResetKeyboardHook()
{
	//MessageBox(NULL,"Resetting Hook","woo", MB_OK);
	if(UnSetKeyboardHook() == true)
        SetKeyboardHook();
}

// Added 2/26/2004
// Adds playlists to the, uh, playlist
// I thought I put this in long ago. I guess I didnt...
void AddLists(vector<string> *tokens, int numRand)
{
	vector<string> *files;

	files = Search(tokens, false, true);
	if(files->size() > 0)
	{
		AddSongsUtility(files, numRand, false);
	}
}

// Added 2/27/2004
// Given a number as an argument, jumps to that number song in the playlist
void JumpToNum(vector<string> *tokens)
{
	int jumpToIndex;
	if (tokens->size() > 1)
	{
        sscanf((*tokens)[1].c_str(),"%i", &jumpToIndex);
		if(jumpToIndex > 0)
		{
            SetPlaylistIndex(jumpToIndex-1);
			if(IsPlaying())
				HitPlayButton();
		}
        
	}
}

// Turns the volume up a little
// There is a message for Winamp 5.x that turns the volume up,
// but I'm not sure if it works in 2.x so I'm not going to use it
void VolumeUp()
{
	int volume = GetVolume();
	volume += 10;
	if(volume > 255)
		volume = 255;

	SetVolume(volume);
}

// Turns the volume down a little
// There is a message for Winamp 5.x that turns the volume down,
// but I'm not sure if it works in 2.x so I'm not going to use it
void VolumeDown()
{
	int volume = GetVolume();
	volume -= 10;
	if (volume < 0)
		volume = 0;

	SetVolume(volume);
}


// TODO: Put this somewhere that makes sense!
// When deleting a file, I need to
// wait for Winamp to process the messages I
// sent before the file can be manipulated
// So, I have this clone of the VB6 DoEvents function
void DoEvents (HWND hWnd)
{
     MSG msg;
     while(PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE))
     {
          GetMessage(&msg, hWnd, 0, 0);
          DispatchMessage(&msg);
     }
}

// Deletes the actual file for the current song
// Sounds scary, but should be okay... Files go to the
// recycling bin
void DeleteCurrentSong()
{
	char file[MAX_PATH];
	int size = GetPlaylistSize();
	int pos; 
	

	if(size == 0)
		return; // Nothing to delete
	
	pos = GetCurrentSongIndex();
	strcpy(file,GetSongFile(pos));

	HitNextButton();

	if(pos == size-1) // Last Song
		HitStopButton();

	// Flush the event queue, so the above events actually take place
	// before I try to delete the file
	DoEvents(g_hwndWinamp);

	DeletePlaylistEntry(pos);

	if(PathIsNetworkPath(file))
	{
		char newfile[MAX_PATH];
		strcpy(newfile,file);
		strcat(newfile,".deleted");

		file[strlen(file)+1] = '\0';
		newfile[strlen(newfile)+1] = '\0';

		SHFILEOPSTRUCT fileOp;
		ZeroMemory((void*)&fileOp,sizeof(SHFILEOPSTRUCT));
		fileOp.hwnd = g_hwndWinamp;
		fileOp.wFunc = FO_RENAME;
		fileOp.fFlags = FOF_RENAMEONCOLLISION | FOF_NOCONFIRMATION ;
		fileOp.pFrom = file;
		fileOp.pTo = newfile;

		SHFileOperation(&fileOp);
	}
	else
	{

		file[strlen(file)+1] = '\0';

		SHFILEOPSTRUCT fileOp;
		ZeroMemory((void*)&fileOp,sizeof(SHFILEOPSTRUCT));
		fileOp.hwnd = g_hwndWinamp;
		fileOp.wFunc = FO_DELETE;
		fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION ;
		fileOp.pFrom = file;

		SHFileOperation(&fileOp);
	}
		
}

void PlayLocation(string location)
{
	HitStopButton();
	ClearPlaylist();
	AddSongToPlaylist(location);
	HitPlayButton();	
}

void AddLocation(string location)
{
	AddSongToPlaylist(location);	
}