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

#include <vector>
#include <string>
#include "basicWinampCommands.h"
#include "commands.h"
#include "sneakyamp.h"
#include "filescan.h"
using namespace std;

//extern intopt *IntOptions;
extern vector<string> g_vFilenames;

vector<string>* Search(vector<string> *tokens, bool media, bool lists)
{
	string search_string = "*";
			
	// Build the search string
	for(int i = 1; i < (int)tokens->size(); i++)
	{	
		search_string += (*tokens)[i];
		search_string +="*";
	}
	
	// See if we want to assum an mp3 extesion,
	// and if so, add it
	//if(ext == "")
	//{
		if (IntOptions[INT_ASSUME_MP3].Edit == TRUE)
			search_string += ".mp3";
	//}
	//else
	//	search_string += ext;

	//MessageBox(NULL, search_string.c_str(), "test", MB_OK);

	FileSearch(search_string.c_str(), media, lists);

	//MessageBox(NULL, g_vFilenames[0].c_str(), "test", MB_OK);


	return &g_vFilenames;

}

// Utility function, adds the files we found to Winamp
void AddSongsUtility(vector<string> *files, int numRand, bool hitPlay)
{
	int rnd;

	if(numRand != 0)
	{
		for(int i = 0; (i < numRand) && !files->empty(); i++)
		{
			rnd = rand()%(int)files->size();
			AddSongToPlaylist((*files)[rnd]);
			files->erase(files->begin() + rnd);
			if(hitPlay && (i == 0))
				HitPlayButton();
		}
	}
	else
	{
		for(int i = 0; i < (int)files->size(); i++)
		{
			AddSongToPlaylist((*files)[i]);
			if(hitPlay && i == 0)
				HitPlayButton();
		}
	}
}

void PlaySongs(vector<string> *tokens, int numRand)
{
	vector<string> *files;

	files = Search(tokens);
	if(files->size() > 0)
	{
		HitStopButton();
		ClearPlaylist();

		AddSongsUtility(files, numRand, true);
	}
}

void AddSongs(vector<string> *tokens, int numRand)
{
	vector<string> *files;

	files = Search(tokens);
	if(files->size() > 0)
	{
		//HitStopButton();
		//ClearPlaylist();

		AddSongsUtility(files, numRand, false);
	}
}

void PlayLists(vector<string> *tokens, int numRand)
{
	vector<string> *files;

	files = Search(tokens, false, true);
	if(files->size() > 0)
	{
		HitStopButton();
		ClearPlaylist();

		AddSongsUtility(files, numRand, true);
	}
}