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

	string inttostr(int n, vector<char> alphabet)
	{
		if (n == 0)
			return alphabet[0] + "";

		int r = alphabet.size();
		string ret = "";
		int bit = 0;
		while (n > 0)
		{
			bit = n % r;
			ret = alphabet[bit] + ret;
			n = n / r;
		}
		return ret;
	}
	
	string _rev_encode(string code, vector<char> alphabet)
	{
		int n = code.length();
		return n == 1 ? code : _rev_encode(inttostr(n - 2, alphabet), alphabet) + code;
	}

	string rev_encode(string code, vector<char> alphabet)
	{
		return _rev_encode(alphabet[1] + code, alphabet) + alphabet[0];
	}
	
	string cw_encode(int cW, char currentChar, vector<char> A, vector<char> E)
	{
		int pos = std::find(A.begin(), A.end(), currentChar) - A.begin();
		return rev_encode(inttostr(cW, E), E) + rev_encode(inttostr(pos, E), E);
	}

	string encode(string text, vector<char> A, vector<char> E)
	{
		string txt = text + "$";
		map<int, string> dictionary;
		int dicIndex = 1;
		int codeWordForPrefix;
		string prefix = "";
		char currentChar;

		string code = "";

		dictionary.insert(pair<int, string>(0, ""));
		for (int i = 0; i < txt.length(); i++)
		{
			if (searchValue(dictionary, txt[i]+""))
			{
				prefix += txt[i];
			}
			else
			{
				if (prefix == "")
				{
					codeWordForPrefix = 0;
				}
				else
					codeWordForPrefix = dicIndex;

				code += cw_encode(codeWordForPrefix, txt[i], A, E);
				
				dictionary.insert(pair<int, string>(dicIndex, txt[i]+""));
				dicIndex++;
				prefix = "";
			}
		}
		return code;
	}

	int strtoint(string s, vector<char> alphabet)
	{
		int r = alphabet.size();
		int rPow = 1;
		int n = 0;

		int pos;

		for (int j = s.length()-1; j > -1; j--)
		{
			pos = std::find(alphabet.begin(), alphabet.end(), s[j]) - alphabet.begin();
			n += pos * rPow;
			rPow *= r;
		}
		return n;
	}

	pair<int, int> cw_decode(string code, vector<char> E)
	{
		int k = 1;
		int j = 0;

		string y = "";

		pair<int, int> ret;		

		while (j == 0 || code[j] != E[0])
		{
			y = code.substr(j, j + k);
			j += k;
			k = strtoint(y.substr(1, y.length()), E) + 2;
		}
		ret.first = strtoint(y.substr(1, y.length()), E);
		ret.second = j + 1;
		return ret;
	}

	string decode(string code, vector<char> A, vector<char> E)
	{
		string txt = "";
		vector<pair<int, int>> dictionary;
		dictionary.push_back(pair<int, int>(0, 0));
		int dicIndex = 1;		

		pair<int, int> pL, kL;

		for (int i = 0; i < code.length();)
		{
			pL = cw_decode(code.substr(i, code.length()), E);
			i += pL.second;
			kL = cw_decode(code.substr(i, code.length()), E);
			i += kL.second;
			txt += (txt.substr(dictionary[pL.first].first, dictionary[pL.first].second) + A[kL.first]);
			dictionary.push_back(pair<int,int>(txt.length() - ((dictionary[pL.first].second - dictionary[pL.first].first) + 1), txt.length()));
		}
		return txt.substr(0, txt.length() - 1);
	}

};

int main() {
	LZ78 lz78;

	/*vector<char> bla, bla2;
	bla.push_back('0');
	bla.push_back('1');
	bla2.push_back('1');
	bla2.push_back('6');

	string code2 = lz78.inttostr(16, bla);
	printf("teste: %s = %d\n", code2.c_str(), lz78.strtoint(code2, bla));
	string rcode = lz78.rev_encode(code2, bla);
	printf("rcode = %s\n", rcode.c_str());
	pair<int, int> pL = lz78.cw_decode(rcode, bla);
	printf("p = %d, l = %d\n", pL.first, pL.second);*/


	string txt = "aabcbcbcbacbabcbabccbabb";
	vector<char> alphabet, eAlphabet;
	alphabet.push_back('a');
	alphabet.push_back('b');
	alphabet.push_back('c');
	eAlphabet.push_back('0');
	eAlphabet.push_back('1');
	
	string code = lz78.encode(txt, alphabet, eAlphabet);
	printf("code= %s\n", code.c_str());

	//string decode = lz78.decode(code, alphabet, eAlphabet);
	//printf("txt= %s\n", decode.c_str());

	return 0;
}

