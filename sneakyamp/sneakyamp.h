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

#ifndef SNEAKYAMP_H
#define SNEAKYAMP_H

#include <afx.h>
#include <vector>
#include <string>

using namespace std;

#include "gen.h"
#include "resource.h"

#include "khk.h"

#define NUM_OF_STRS         45
#define NUM_OF_INTS			8

#define TRUE				1
#define FALSE				0

#define STR_BEGIN_COM		0
#define STR_END_COM			1
#define STR_PLAY_COM		2
#define STR_ADD_COM			3
#define STR_STOP_COM		4
#define STR_PAUSE_COM		5
#define STR_FORWARD_COM		6
#define STR_PREVIOUS_COM	7
#define STR_NEXT_COM		8
#define STR_VOLUME_COM		9
#define STR_BALANCE_COM		10
#define STR_REWIND_COM		11
#define STR_SHUFFLE_COM		12
#define STR_REPEAT_COM      13
#define STR_ON_COM			14
#define STR_OFF_COM			15
#define STR_RANDOM_COM		16
#define STR_DIR1			17
#define STR_DIR_PREFIX		18
#define STR_DIR2            19
#define STR_DIR3			20
#define STR_DIR4			21
#define STR_PLAYLIST_COM    22
#define STR_PLAYCD_COM      33
#define STR_JUMP_COM		34
#define STR_SHIFT_COM		35
#define STR_ADDNEXT_COM		36
#define STR_ADDLIST_COM		37
#define STR_JUMPTONUM_COM	38
#define STR_DELETEFILE_COM	39
#define STR_MEDIAFILETYPES	40
#define STR_LISTFILETYPES	41
#define STR_PLAYPAUSE_COM	42
#define STR_PLAYLOC_COM		43
#define STR_ADDLOC_COM		44

#define INT_ENABLED			0
#define INT_BEGIN_COM_STR	1
#define INT_BEGIN_COM_ENTER	2
#define INT_END_COM_STR		3
#define INT_END_COM_ENTER	4
#define INT_ASSUME_MP3		5
#define INT_OFFSET			6
#define INT_MAX_FILES		7

#define IPC_PLAYFILE 100

// IDs for all of the Hotkeys I'll be registering
enum {	MEDIA_VOLUME_MUTE,  
		MEDIA_VOLUME_DOWN ,      
		MEDIA_VOLUME_UP    ,  
		MEDIA_NEXT_TRACK    ,
		MEDIA_PREV_TRACK  ,
		MEDIA_STOP        ,
		MEDIA_PLAY_PAUSE
	};


void config();
void quit();
int init();

struct stropt {
	string Description;
	string Edit;
	string Default;
	int ID;
}; // Number of String options

struct intopt {
	string Description;
	int Edit;
	int Default;
	int ID;
};

#include "dirscan.h"

// Used to identify if this is the first time SneakyAmp has run
extern int first_start;

// Used to identify if the keyboard hook has been set or not.
//extern int hhook;

// Application name
extern char szAppName[];

#endif

