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
 
#include "inputparse.h"
#include "playlist.h"
#include "commands.h"
#include "newCommands.h"
#include "basicWinampCommands.h"
#include "stringutil.h"

string input;


// Special: If this gets an input of '0', begin and end command parsing is skipped
void inputParse(char input_char)
{
	//char buffer[255];

	// Strings for the begin and end commands,
	// needed in case one is only (CHAR) 13,
	// in which case using the edit box wouldnt be
	// accurate
	string begin_com, end_com;

	// Ints to store the positions of the
	// begin and end commands in the input
	// string, if they're found
	int end_find, begin_find;
	
	// Vector for storing our string tokens
	vector<string> tokens;

	// Random data - how many random songs the user wants
	int random_data = 0;

	// Search string
	string search_string;
	
	///////////////////////////////////////////////////////////////////////////

	// Backspace
	if(input_char == 8)
	{
		input = Left(input, (int)input.length() -1);
		return;
	}
	
	if(input_char != 0) //If char is 0, we want to go straight to command parsing
	{
		// Add new char to input string
		input += input_char;

		//MessageBox(NULL,input.c_str(),"feh",MB_OK);

		// Check if we're using the edit box, 
		// or (CHAR) 13 for the end command/
		// and set accordingly
		if (IntOptions[INT_END_COM_ENTER].Edit == TRUE)
			end_com = (CHAR) 13; 
		else
			end_com = StringOptions[STR_END_COM].Edit;

		// Check if we're using the edit box, 
		// or (CHAR) 13 for the begin command
		// and set accordingly
		if (IntOptions[INT_BEGIN_COM_ENTER].Edit == TRUE)
			begin_com = (CHAR) 13;
		else
			begin_com = StringOptions[STR_BEGIN_COM].Edit;

		// See if the begin and end commands are in the input string
		end_find = (int)input.find(end_com);
		begin_find = (int)input.find(begin_com);

		// If the end command isnt found, 
		// then there is nothing to do
		if (end_find == -1)
			return;

		// If the end command was found, but no 
		// begin command, then we can toss our
		// input string and return
		if (begin_find == -1)
		{
			input = "";
			return;
		}

		// Strip the end command and anything after it,
		// which should be nothing
		input = Left(input, (int)(input.length() - end_com.length()));

		// In case there are more than 1 begin commands
		// in the string, strip the begin command and
		// anything before it, until no more begin commands
		// are left
		while(begin_find != -1)
		{	
			input = Right(input, (int)(input.length() - begin_com.length() - begin_find));
			begin_find = (int)input.find(begin_com);
		}
	}
	
	// Now we're left with whatever the user is trying 
	// to tell us, so we can stuff the input string
	// through a string tokenizer
	StrTokenize(&tokens, &input, ' ');


	// If no tokens were returned, then we can clear the
	// input string and return
	if(tokens.size() == 0)
	{
		input = "";
		return;
	}

	// Clear out old directory tokens
	dir_tokens.clear();
	
	// Filter out the tokens with the directory prefix
	for(UINT i = 0; i < tokens.size(); i++)
	{
		if(tokens[i].find(StringOptions[STR_DIR_PREFIX].Edit) == 0)
		{
			// Strip prefix off the token
			tokens[i] = tokens[i].substr(StringOptions[STR_DIR_PREFIX].Edit.length(), tokens[i].length()-StringOptions[STR_DIR_PREFIX].Edit.length());
			dir_tokens.push_back(tokens[i].c_str());
			tokens.erase(tokens.begin() +i);
			i--; // Decrement i so this position is checked again
		}
	}


	// Filter out random information
	for(i = 0; i < tokens.size()-1; i++)
	{
		
		// Check if token has random command in it
		if(StringOptions[STR_RANDOM_COM].Edit.find(tokens[i]) != -1)
		{
//MessageBox(NULL,input.c_str(),"feh",MB_OK);
			// Make sure next token is a number
			if(isNumber(tokens[i+1].c_str()))
			{

				// Remove the random command from the token list
				tokens.erase(tokens.begin() +i);

				// Get the random data
				random_data = atoi(tokens[i].c_str());

				// Remove the random data from the token list
				tokens.erase(tokens.begin() +i);
				
				// Decrement i to this position is checked again
				i--;
			}
		}
	}
	

	//sprintf(buffer,"rand_data: %i",random_data);
	
	//MessageBox(plugin.hwndParent,buffer,"results",MB_OK);

	tokens[0] = MakeLower(tokens[0]);

	// Check if user typed the play command
	if (StringOptions[STR_PLAY_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// If there is only one token, all we want to do is
		// to push Winamp's play button
		if (tokens.size() == 1)
			HitPlayButton();
			//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);
		else // Otherwise, we need to see what songs to play
		{
			/*
			search_string = "*";
			
			// Build the search string
			for(i = 1; i < tokens.size(); i++)
			{	
				search_string += tokens[i];
				search_string +="*";
			}
			
			// See if we want to assum an mp3 extesion,
			// and if so, add it
			if (IntOptions[INT_ASSUME_MP3].Edit == TRUE)
				search_string += ".mp3";
			

			FileSearch(search_string.c_str());
			*/
			
			PlaySongs(&tokens, random_data);
			
			// Set the dwData entry of the COPYDATASTRUCT to 
			// let Winamp know what we're sending
			//cds.dwData = IPC_PLAYFILE;
			
			/*
			if (!random)
			{
				for(int i = 0; i<(int)g_vFilenames.size() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[i].GetBuffer(MAX_PATH);
					//g_vFilenames[i].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null		
						
					if (i == 0) // If this is the first song to play...
					{
						// Stop Winamp
						HitStopButton();
						// Clear the playlist
						ClearPlaylist();
					
						// Send the song to Winamp to play
						AddSongToPlaylist(g_vFilenames[i].c_str() );
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
						
						// Slap the play button
						HitPlayButton();
					}
					else
					{
						// This isnt the first song, so just send the COPYDATASTRUCT
						// over to Winamp, to add the song to the playlist
						AddSongToPlaylist(g_vFilenames[i].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
					}	
				}
			}
			else // The user wants to play a random subset...
			{
				for(int i = 0;  i < random_data && !g_vFilenames.empty() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Pick a song to play
					random_index = rand()%((int)g_vFilenames.size());

					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[random_index].GetBuffer(MAX_PATH);
					//g_vFilenames[random_index].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null
						
					if (i == 0) // If this is the first song to play...
					{
						// Stop Winamp
						HitStopButton();

						// Clear the playlist
						ClearPlaylist();
						
						// Send the song data over to Winamp
						AddSongToPlaylist(g_vFilenames[random_index].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	

						// Poke the play button
						HitPlayButton();
						//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);	
					}
					else
					{
						// Send the song to Winamp
						AddSongToPlaylist(g_vFilenames[random_index].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	
					}

					// Remove filename entry from our vector
					g_vFilenames.erase(g_vFilenames.begin() + random_index);
										
				}
			}*/
		}
	}
	else if (StringOptions[STR_STOP_COM].Edit.find(tokens[0].c_str()) != -1)
		// User sent stop command, so hit the stop button
		HitStopButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40047, 0); 
	else if (StringOptions[STR_NEXT_COM].Edit.find(tokens[0].c_str()) != -1)
		// User sent next command, so hit the next button
		HitNextButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40048, 0);
	else if (StringOptions[STR_PREVIOUS_COM].Edit.find(tokens[0].c_str()) != -1)
		// User sent previous command, so hit the previous button
		HitPrevButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40044, 0);
	else if (StringOptions[STR_PAUSE_COM].Edit.find(tokens[0].c_str()) != -1)
		// User sent pause command, so hit the pause button
		HitPauseButton();
		//PostMessage(plugin.hwndParent,WM_COMMAND,40046, 0);
	else if (StringOptions[STR_VOLUME_COM].Edit.find(tokens[0].c_str()) != -1)
	{   
		// User wants to adjust the volume
		if (tokens.size() > 1 && isNumber(tokens[1]))
		{
			// Get the volume setting from the next token
			int data = atoi(tokens[1].c_str());
			if ((data >= 0) && (data <=255))
				// Tel winamp what the user would like the volume set to
				SetVolume(data);
				//PostMessage(plugin.hwndParent,WM_USER, data, 122);
		}
	} 
	else if (StringOptions[STR_BALANCE_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		if (tokens.size() > 1)
		{
			int data;
			// Get balance setting from next token, using
			// sscanf to catch negative numbers
			// ***If the directory prefix is '-', no *** 
			// ***negative numbers will be caught    ***
			sscanf(tokens[1].c_str(),"%i", &data);

			if ((data >= -128) && (data <=128))
				// Tell Winamp where we want the balance set to
				SetBalance(data);
				//SendMessage(plugin.hwndParent,WM_USER, data, 123);
		}
	}
	else if (StringOptions[STR_FORWARD_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User sent forward command, wants to advance the song position

		// Get current song position
		int data = GetSongPosition();//SendMessage(plugin.hwndParent,WM_USER,0,105);
		if (data != -1) // If song is playing...
		{
			if (tokens.size() > 1)
			{
				// Calculate where the song position should be,
				// using the number in the next token
				data = data + (atoi(tokens[1].c_str()) * 1000);

				// Set song position
				SetSongPosition(data);
				//SendMessage(plugin.hwndParent,WM_USER,data,106);
			}
			else
			{
				// User didnt specify an offset, so add the 
				// default offset to the song position
				data = data + (IntOptions[INT_OFFSET].Edit * 1000);

				// Set song position
				SetSongPosition(data);
				//SendMessage(plugin.hwndParent,WM_USER,data,106);
			}
		}
	}
	else if (StringOptions[STR_REWIND_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User send the rewind command, wants to rewind the song position

		// Get current song position
		int data = GetSongPosition();//SendMessage(plugin.hwndParent,WM_USER,0,105);
		if (data != -1) // If song is playing...
		{
			if (tokens.size() > 1)
			{
				// Calculate where the song position should be,
				// using the number in the next token
				data = data - (atoi(tokens[1].c_str()) * 1000);

				// Set song position
				SetSongPosition(data);
				//SendMessage(plugin.hwndParent,WM_USER,data,106);
			}
			else
			{
				// User didnt specify an offset, so subtract the 
				// default offset from the song position
				data = data - (IntOptions[INT_OFFSET].Edit * 1000);

				// Set song position
				SetSongPosition(data);
				//SendMessage(plugin.hwndParent,WM_USER,data,106);
			}
		}
	}
	else if (StringOptions[STR_SHUFFLE_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User wants to set or toggle the shuffle button

		// Get the shuffle button's current state
		//int data = SendMessage(plugin.hwndParent,WM_USER,0,250);

		// If no other tokens were sent...
		if (tokens.size()== 1)
		{
			// Toggle the shuffle button
			//PostMessage(plugin.hwndParent,WM_COMMAND,40023, 0);
			ToggleRandom();
		}
		else if (StringOptions[STR_ON_COM].Edit.find(tokens[1].c_str()) != -1) 
		{
			// User wants to turn the shuffle button on
			TurnRandomOn();

			//if (data == 0)
				// If the shuffle button is off, toggle it
				//PostMessage(plugin.hwndParent,WM_COMMAND,40023, 0);
		}
		else if (StringOptions[STR_OFF_COM].Edit.find(tokens[1].c_str()) != -1) 
		{
			// User wants to turn the shuffle button off
			TurnRandomOff();

			//if (data == 1)
				// If the shuffle button is on, toggle it
				//PostMessage(plugin.hwndParent,WM_COMMAND,40023, 0);
		}
	}
	else if (StringOptions[STR_REPEAT_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User wants to set or toggle the repeat button

		// Get the current state of the repeat button
		//int data = SendMessage(plugin.hwndParent,WM_USER,0,251);

		// If no other tokens were sent...
		if (tokens.size() ==1)
		{
			// Toggle the repeat button
			ToggleRepeat();
			//PostMessage(plugin.hwndParent,WM_COMMAND,40022, 0);
		}
		else if (StringOptions[STR_ON_COM].Edit.find(tokens[1].c_str()) != -1) 
		{
			// User wants to turn the repeat button on
			TurnRepeatOn();
			//if (data == 0)
				// If the repeat button is off, toggle it
				//PostMessage(plugin.hwndParent,WM_COMMAND,40022, 0);
		}
		else if (StringOptions[STR_OFF_COM].Edit.find(tokens[1].c_str()) != -1) 
		{
			// User wants to turn off the repeat button
			TurnRepeatOff();

			//if (data == 1)
				// If the repeat button is on, toggle it
				//PostMessage(plugin.hwndParent,WM_COMMAND,40022, 0);
		}
	}
	else if (StringOptions[STR_ADD_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User wants to add songs to the playlist

		// Make sure there are search tokens 
		if (tokens.size() > 1)
		{
			AddSongs(&tokens, random_data);
			/*
			search_string = "*";
			
			// Build the search string
			for(i = 1; i < tokens.size(); i++)
			{	
				search_string += tokens[i];
				search_string +="*";
			}
			
			// See if we want to assum an mp3 extesion,
			// and if so, add it
			if (IntOptions[INT_ASSUME_MP3].Edit == TRUE)
				search_string += ".mp3";
		

			FileSearch(search_string.c_str());
			
			// Set the dwData entry of the COPYDATASTRUCT to 
			// let Winamp know what we're sending
			//cds.dwData = IPC_PLAYFILE;
			
			if (!random)
			{
				for(int i = 0; i<(int)g_vFilenames.size() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[i].GetBuffer(MAX_PATH);
					//g_vFilenames[i].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null		
						
					
					// Send the COPYDATASTRUCT over to Winamp, to add the song to the playlist
					AddSongToPlaylist(g_vFilenames[i].c_str());
					//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	
				}
			}
			else // The user wants to play a random subset...
			{
				for(int i = 0;  i < random_data && !(int)g_vFilenames.empty() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Pick a song to add
					random_index = rand()%((int)g_vFilenames.size());

					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[random_index].GetBuffer(MAX_PATH);
					//g_vFilenames[random_index].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null
						
					// Send the song to Winamp
					AddSongToPlaylist(g_vFilenames[random_index].c_str());
					//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	

					// Remove the entry from the filename vector
					g_vFilenames.erase(g_vFilenames.begin() + random_index);					
				}
				
			}*/
		}
	}
	else if (StringOptions[STR_PLAYLIST_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User wants to play a playlist

		// Make sure we;ve got more than just the command to work with
		if (tokens.size() > 1)
		{
			PlayLists(&tokens, random_data);
			/*
			search_string = "*";
			
			// Build the search string
			for(i = 1; i < tokens.size(); i++)
			{	
				search_string += tokens[i];
				search_string +="*";
			}
			
			// Since we're looking for playlists, add the playlist extension
			search_string += ".m3u";
		
			FileSearch(search_string.c_str());
			
			// Set the dwData entry of the COPYDATASTRUCT to 
			// let Winamp know what we're sending
			//cds.dwData = IPC_PLAYFILE;
			
			if (!random)
			{
				for(int i = 0; i<(int)g_vFilenames.size() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[i].GetBuffer(MAX_PATH);
					//g_vFilenames[i].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null		
						
					if (i == 0) // If this is the first song to play...
					{
						// Stop Winamp
						HitStopButton();
						//PostMessage(plugin.hwndParent,WM_COMMAND,40047, 0);

						// Clear the playlist
						ClearPlaylist();
						//SendMessage(plugin.hwndParent,WM_USER, 0, 101);
					
						// Send the song to Winamp to play
						AddSongToPlaylist(g_vFilenames[i].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
						
						// Slap the play button
						HitPlayButton();
						//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);
					}
					else
					{
						// This isnt the first song, so just send the COPYDATASTRUCT
						// over to Winamp, to add the song to the playlist
						AddSongToPlaylist(g_vFilenames[i].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
					}	
				}
			}
			else // The user wants to play a random subset...
			{
				for(int i = 0;  i < random_data && !g_vFilenames.empty() && i < IntOptions[INT_MAX_FILES].Edit; i++)
				{
					// Pick a song to play
					random_index = rand()%((int)g_vFilenames.size());

					// Set cds.lpData to the filename of the current song
					//cds.lpData = (void *) g_vFilenames[random_index].GetBuffer(MAX_PATH);
					//g_vFilenames[random_index].ReleaseBuffer();

					// Set the length of the current song's filename
					//cds.cbData = strlen((char *) cds.lpData)+1; // include space for null
						
					if (i == 0) // If this is the first song to play...
					{
						// Stop Winamp
						HitStopButton();
						//PostMessage(plugin.hwndParent,WM_COMMAND,40047, 0);

						// Clear the playlist
						ClearPlaylist();
						//SendMessage(plugin.hwndParent,WM_USER, 0, 101); 
						
						// Send the song data over to Winamp
						AddSongToPlaylist(g_vFilenames[random_index].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	

						// Poke the play button
						HitPlayButton();
						//PostMessage(plugin.hwndParent,WM_COMMAND,40045, 0);	
					}
					else
					{
						// Send the song to Winamp
						AddSongToPlaylist(g_vFilenames[random_index].c_str());
						//SendMessage(plugin.hwndParent,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);	
					}

					// Remove filename entry from our vector
					g_vFilenames.erase(g_vFilenames.begin() + random_index);
										
				}
			}*/

		}
	}
	else if (StringOptions[STR_PLAYCD_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// User send the play cd command, wants to star a cd playing


		
		if (tokens.size() > 1)// User specified a cd drive number
		{
			int cd = atoi(tokens[1].c_str());

			PlayCD(cd);

			//if (cd > 0 && cd < 8) // dont let user go crazy
			//	                  // limit picked at random, check this out for weird stuff.
			//{
			//	// Pick the Play CD menu item (I hope)
			//	PostMessage(plugin.hwndParent,WM_COMMAND,40322 + cd, 0);
			//}
		}
		else // User didnt specify a cd drive number
		{
			
			// Pick the Play CD menu item, for first CD drive (I hope)
			PlayCD();
			//PostMessage(plugin.hwndParent,WM_COMMAND,40323, 0);
		}
		
	}
	else if (StringOptions[STR_JUMP_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		JumpToFile(&tokens);
		
	}
	else if (StringOptions[STR_SHIFT_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		ShiftSongsToNext(&tokens);
		
	}
	else if (StringOptions[STR_ADDNEXT_COM	].Edit.find(tokens[0].c_str()) != -1)
	{
		AddSongsToNext(&tokens);
		
	}
	// Added 2/26/2004
	else if (StringOptions[STR_ADDLIST_COM	].Edit.find(tokens[0].c_str()) != -1)
	{
		// Make sure we;ve got more than just the command to work with
		if (tokens.size() > 1)
		{
			AddLists(&tokens, random_data);
		}
	}
	// Added 2/27/2004
	else if (StringOptions[STR_JUMPTONUM_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		// Make sure we;ve got more than just the command to work with
		if (tokens.size() > 1)
		{
			JumpToNum(&tokens);
		}
	}
	// Disabling this command until I feel better about it
	//else if (StringOptions[STR_DELETEFILE_COM].Edit.find(tokens[0].c_str()) != -1)
	//{
	//	DeleteCurrentSong();
	//}
	else if (StringOptions[STR_PLAYPAUSE_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		HitPlayPause();
	}
	else if (StringOptions[STR_PLAYLOC_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		if (tokens.size() > 1)
		{
			PlayLocation(tokens[1]);
		}
	}
	else if (StringOptions[STR_ADDLOC_COM].Edit.find(tokens[0].c_str()) != -1)
	{
		if (tokens.size() > 1)
		{
			AddLocation(tokens[1]);
		}
	}
	else
	{
		// User sent us gibberish, clear the input string
		input = "";
	}

	return;
}
