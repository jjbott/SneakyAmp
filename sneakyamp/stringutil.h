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

string Left(const string s, int numChars);
string Right(const string s, int numChars);
string MakeLower(const string s);
bool StringMatch(const string*, vector<string>*);
bool isNumber(string test_string);
void StrTokenize(vector<string> *outputVector, string *inputString, char Delimiter);
