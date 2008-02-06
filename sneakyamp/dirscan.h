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

#ifndef DIRSCAN_H
#define DIRSCAN_H

#include "sneakyamp.h"

void SetDirectoryStructure(); 

extern winampGeneralPurposePlugin plugin;

extern vector<string> directories;

extern stropt StringOptions[NUM_OF_STRS];
extern intopt IntOptions[NUM_OF_INTS];

#endif 