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

#include "sneakyamp.h"

void FileSearch(const string searchkey, bool media = true, bool lists = false);


extern vector<string> g_vFilenames;

extern vector<string> directories;

extern stropt StringOptions[NUM_OF_STRS];
extern intopt IntOptions[NUM_OF_INTS];
extern vector<string> dir_tokens;