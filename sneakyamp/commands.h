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

#include <vector>
#include <string>
using namespace std;

void AddSongsUtility(vector<string> *files, int numRand = 0, bool hitPlay = false);
void PlaySongs(vector<string> *files, int numRand = 0);
void AddSongs(vector<string> *tokens, int numRand = 0);
void PlayLists(vector<string> *tokens, int numRand = 0);
vector<string>* Search(vector<string> *tokens, bool media = true, bool lists = false);