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

#include "dirscan.h"
#include "stringutil.h"

void SetDirectoryStructure() 
{
	// Handle to directories we've found
	HANDLE hFind;

	// Data for directories we've found
	WIN32_FIND_DATA FindFileData;
	
	// For loop counter
	int i;

	// Temporary directory string
	string temp_directory;

	// Storage for current directory
	char currentDir[MAX_PATH];

	///////////////////////////////////////////////////////////////////////

	// Get current directory, so we can reset it
	currentDir[0] = '\0'; // clear string, sortof
	GetCurrentDirectory(MAX_PATH,currentDir);

	// Get rid of old directories 
	directories.clear();
	

	// Add the base directory 
	directories.push_back(StringOptions[STR_DIR1].Edit);

	// If there are more mp3 directories defined, add them also
	if(StringOptions[STR_DIR2].Edit != "")
		directories.push_back(StringOptions[STR_DIR2].Edit);
	if(StringOptions[STR_DIR3].Edit != "")
		directories.push_back(StringOptions[STR_DIR3].Edit);
	if(StringOptions[STR_DIR4].Edit != "")
		directories.push_back(StringOptions[STR_DIR4].Edit);

	for(i = 0; i < (int)directories.size(); i++)
	{
		// Set the current directory to directories[i]
		if (SetCurrentDirectory( directories[i].c_str() ) != 0)
		{
			// Find the first directory in this dorectory
			hFind = FindFirstFile( "*", &FindFileData );

			//MessageBox(plugin.hwndParent,"'"+directories[i]+"'","results",MB_OK);

			// if we found something...
			if (hFind != INVALID_HANDLE_VALUE)
			{
				// Make sure we found a directory...
				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{
					// Make sure we dont pick up the '.' and '..' directories
					if ((strcmp(FindFileData.cFileName, ".") != 0) && (strcmp(FindFileData.cFileName, "..") != 0))
					{
						// Copy the directory name into our temporary string
						temp_directory = directories[i];
						
						// Make sure it ends with '\'
						if (Right(temp_directory,1) != "\\")
							temp_directory += "\\";

						// Add the found directory name to our temp string
						temp_directory += FindFileData.cFileName;
						
						// Change it to lower case, to make finding specific 
						// directories in the File Scan easier
						MakeLower(temp_directory);
						//if (temp_directory != directories[i])
							directories.push_back(temp_directory);
						//MessageBox(plugin.hwndParent,"'"+temp_directory+"'","results",MB_OK);
					}
				}
				
				// while there are more directories to be found...
				while(FindNextFile( hFind, &FindFileData ) !=0)
				{	
					// Make sure we found a directory
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
					{
						// Make sure we dont pick up the '.' and '..' directories
						if ((strcmp(FindFileData.cFileName, ".") != 0) && (strcmp(FindFileData.cFileName, "..") != 0))
						{
							// Copy the directory name into our temporary string
							temp_directory = directories[i];
						
							// Make sure it ends with '\'
							if (Right(temp_directory, 1) != "\\")
								temp_directory += "\\";

							// Add the found directory name to our temp string
							temp_directory += FindFileData.cFileName;
						
							// Change it to lower case, to make finding specific 
							// directories in the File Scan easier
							MakeLower(temp_directory);
							
							directories.push_back(temp_directory);
							//MessageBox(plugin.hwndParent,"'"+temp_directory+"'","results",MB_OK);
						}
					}
				}

				FindClose(hFind);
			}
		}
	}

	if (currentDir[0] != '\0 ')
		SetCurrentDirectory(currentDir);

}