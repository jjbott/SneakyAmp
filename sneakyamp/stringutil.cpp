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
//#include "stringutil.h"
#include <afx.h>
using namespace std;

string Left(const string s, int numChars)
{
	if(numChars >= (int)s.length())
		return s;
	else
		return s.substr(0, numChars);
}

string Right(const string s, int numChars)
{
	if(numChars >= (int)s.length())
		return s;
	else
		return s.substr(s.length()-numChars, numChars);
}

string MakeLower(const string s)
{
	string temp = s;
	for(int i = 0; i < (int)s.length(); i++)
		temp[i] = tolower(s[i]);
	return temp;
}

bool StringMatch(const string *s, vector<string> *searchWords)
{
	int pos = 0;
	string tempstring = *s;
	
	if (searchWords->size() == 0)
		return true;

	tempstring = MakeLower(tempstring);

	for(int i = 0; i < (int)searchWords->size(); i++)
	{
		//string buffer = "looking for \"" + MakeLower((*searchWords)[i]) + "\" in " + tempstring;
		
		pos = (int)tempstring.find(MakeLower((*searchWords)[i]), pos);

		if(pos == -1)
		{
			//buffer += " and failed";
			//MessageBox(NULL, buffer.c_str(), "test", MB_OK);
			return false;
		}
		else
		{
			//buffer += " and found it!";
			//MessageBox(NULL, buffer.c_str(), "test", MB_OK);
			pos += (int)(*searchWords)[i].size();
		}
	}

	return true;

}


bool isNumber(string test_string)
{

	for(int i = 0; i < (int)test_string.length(); i++)
		if(test_string[i] <48 || test_string[i] > 57) return false;

	return true;
}

void StrTokenize(vector<string> *outputVector, string *inputString, char Delimiter)
{
	int pos = 0, cur_pos = 0;
	string tempString;
	
	//char buffer[255];

	outputVector->clear();

	// Find the first instance of the delimiter
	pos = (int)inputString->find(Delimiter, cur_pos);

	// Continue until the entire string is tokenized
	while (pos != -1 && pos < (int)inputString->length())
	{

		if(pos == cur_pos)
			cur_pos++;
		else
		{
			tempString=inputString->substr(cur_pos, pos-cur_pos);
			outputVector->push_back(tempString);
			cur_pos = pos+1;
		}

		pos = (int)inputString->find(Delimiter, cur_pos);
		//sprintf(buffer,"pos: %i\ncur_pos: %i\nlength: %i",pos, cur_pos, inputString->GetLength());
	
		//MessageBox(plugin.hwndParent,buffer,"results",MB_OK);
	}

	if(pos != inputString->length()-1)
	{
		tempString=inputString->substr(cur_pos, inputString->length());
		if (tempString != "")
			outputVector->push_back(tempString);
	}
}