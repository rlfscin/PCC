// LZ78Algoritmo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
using namespace std;

class LZ78 {
public:
	LZ78(){}

	bool searchValue(map<int, string> dic, string value)
	{
		map<int, string>::iterator it;
		for (it = dic.begin(); it != dic.end(); ++it)
		{			
			if (it->second == value)
			{
				return true;
			}
			else
				return false;
		}
		return false;
	}

	string encode(string text)
	{
		map<int, string> dictionary;
		int dicIndex = 1;
		int codeWordForPrefix;
		string prefix = "";
		char currentChar;

		string code = "";
		for (int i = 0; i < text.length(); i++)
		{
			currentChar = text[i];
			string aux = prefix + currentChar;
			if (searchValue(dictionary, aux.c_str()))
			{
				prefix += currentChar;
			}
			else
			{
				if (prefix == "")
				{
					codeWordForPrefix = 0;
				}
				else
					codeWordForPrefix = dicIndex;

//				code = code + '(' + to_string(codeWordForPrefix) + ',' + currentChar + ");";
				
				dictionary.insert(pair<int, string>(dicIndex, aux.c_str()));
				dicIndex++;
				prefix = "";
			}
		}
		if (prefix.length() > 0)
		{
			codeWordForPrefix = dicIndex;
			code += '(' + codeWordForPrefix + ", )";
		}
		return code;
	}

	string decode(string code)
	{
		map<int, string> dictionary;
		int dicIndex = 1;
		int codeWord;
		char correspondingChar;
		//for (int i = 0; i < code.length(); i++)
		//{
		//	codeWord = 
		//}
	}

};

int main() {
	LZ78 lz78;

	string code = lz78.encode("ABBCBCABABCAABCAAB");
	printf("code= %s\n", code.c_str());

	return 0;
}

