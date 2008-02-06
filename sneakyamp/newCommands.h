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

// Dedicated file for commands I'm adding today (9/14/2003)

#pragma once

void JumpToFile(vector<string> *tokens);
bool ShiftSongsToNext(const vector<string> *tokens);
void AddSongsToNext(vector<string> *tokens, int numRand = 0);
void InsertSongsUtility(vector<string> *files, int index, int numRand = 0);

// Added 12/04/2003
void ResetKeyboardHook();

// Added 2/26/2004
void AddLists(vector<string> *tokens, int numRand = 0);

// Added 2/27/2004
void JumpToNum(vector<string> *tokens);

// Added 3/1/2004
void VolumeUp();
void VolumeDown();

// 3/8/2004
void DeleteCurrentSong();

// 4/24/2004
void PlayLocation(string location);

// 4/25/2004
void AddLocation(string location);