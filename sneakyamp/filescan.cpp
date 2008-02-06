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

#include "filescan.h"
#include "stringutil.h"

// function uses known file extentions to determine if
// the file is a media file. Always returns true if filtering is turned off
bool IsMediaFile(_TCHAR* filename)
{
	// TODO: Add filter toggle

	// TODO: This converts the filename to uppercase, in place. 
	// Maybe I shouldnt do that in place... I guess it doesnt really matter
	_tcsupr(filename);

	_TCHAR* extension;
	extension = _tcsrchr(filename,'.');

	if(extension == NULL)
		return false; // No extension

	extension += sizeof(_TCHAR);
	
	// simply look for the extension in the allowed extensions string
	// has the side effect of finding stuff like "mpg" if only "mpg4" is listed though
	// I should probably fix that. So, FIXME
	if(_tcsstr(_T(StringOptions[STR_MEDIAFILETYPES].Edit.c_str()),extension) != NULL)
		return true;

	return false;

}

// function uses known file extentions to determine if
// the file is a playlist file. Always returns true if filtering is turned off
bool IsListFile(_TCHAR* filename)
{
	// TODO: Add filter toggle

	// TODO: This converts the filename to uppercase, in place. 
	// Maybe I shouldnt do that in place... I guess it doesnt really matter
	_tcsupr(filename);

	_TCHAR* extension;
	extension = _tcsrchr(filename,'.');

	if(extension == NULL)
		return false; // No extension

	extension += sizeof(_TCHAR);
	
	// simply look for the extension in the allowed extensions string
	// has the side effect of finding stuff like "mpg" if only "mpg4" is listed though
	// I should probably fix that. So, FIXME
	if(_tcsstr(_T(StringOptions[STR_LISTFILETYPES].Edit.c_str()),extension) != NULL)
		return true;

	return false;

}

void FileSearch(const string searchkey, bool media, bool lists) 
{

	// Handle to files that are found
	HANDLE hFind;

	// Temporary CString used to build g_vFilenames
	string temp_filename;

	// Bool to determine if a directory should be searched or not
	//bool usedir = true;

	// Data for the files we find
	WIN32_FIND_DATA FindFileData;

	// For loop counters
	int i;

	// Storage for current directory
	char currentDir[MAX_PATH];


	//////////////////////////////////////////////////////////////////////////

	// Get current directory, so we can reset it
	currentDir[0] = '\0'; // clear string, sortof
	GetCurrentDirectory(MAX_PATH,currentDir);

	// Clear out the old g_vFilenames
	g_vFilenames.clear();

	// Scan every directory in the directory vector...
	for(i = 0; i < (int)directories.size(); i++)
	{

		//u = *directories.at(i);
		//usedir = true;
		//find1 = 0;
		//find2 = 0;

		// Check to see if the user wants to use this directory...
		//for(j = 0; usedir && j<(int)dir_tokens.size(); j++)
		//{
		//	dir_tokens[j] = MakeLower(dir_tokens[j]);

		//	if ((directories[i].find(dir_tokens[j]) == -1))
				// If the directory token wasnt found in this directory
				// then we dont want to scan it, so we set the 
				// usedir flag to false
		//		usedir = false;
		//}
		
		if (StringMatch(&directories[i], &dir_tokens))//(usedir == true) // If the directory should be scanned...
		{
			// Set the current directory to this directory
			if(SetCurrentDirectory( directories[i].c_str() ) != 0)
			{
				// Find the first matching file
				hFind = FindFirstFile( searchkey.c_str(), &FindFileData );

				// If something was found...
				if (hFind != INVALID_HANDLE_VALUE)
				{
					// If the file we found is not a directory...
					if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
					{
						if( (media && IsMediaFile(FindFileData.cFileName)) ||
								(lists && IsListFile(FindFileData.cFileName)) )
						{
							temp_filename = directories[i];

							// Make sure the filename path ends with a '\'
							if (temp_filename[temp_filename.length()-1] != '\\')
							{
								temp_filename += "\\";
							}
							
							// Add the filename to our temp filename
							temp_filename += FindFileData.cFileName;
							
							// Push the filename into our filename vector
							g_vFilenames.push_back(temp_filename);
							//MessageBox(plugin.hwndParent,"'"+temp_filename+"'","results",MB_OK);
						}
					}	
					
					// While there are more files to be found...
					while (FindNextFile( hFind, &FindFileData ) != 0)
					{	
						// If the file we found is not a directory...
						if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) 
						{
							if( (media && IsMediaFile(FindFileData.cFileName)) ||
								(lists && IsListFile(FindFileData.cFileName)) )
							{
								temp_filename = directories[i];

								// Make sure the filename ends with a '\'
								if (temp_filename[temp_filename.length()-1] != '\\')
								{
									temp_filename += "\\";
								}
							
								// Add the filename to our temp filename
								temp_filename += FindFileData.cFileName;
							
								// Push the filename into our filename vector
								g_vFilenames.push_back(temp_filename);
							}
							
						}
					}

					FindClose(hFind);
				}
			}
		}
	}

	if (currentDir[0] != '\0 ')
		SetCurrentDirectory(currentDir);
}

