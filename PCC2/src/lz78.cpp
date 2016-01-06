// LZ78Algoritmo.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <stdio.h>
#include <bitset>

using namespace std;

class LZ78 {
public:
	LZ78(){}

	// bool searchValue(map<int, string> dic, string value)
	// {
	// 	map<int, string>::iterator it;
	// 	for (it = dic.begin(); it != dic.end(); ++it)
	// 	{			
	// 		if (it->second == value)
	// 		{
	// 			return true;
	// 		}
	// 		else
	// 			return false;
	// 	}
	// 	return false;
	// }

	string inttostr(int n, vector<char> alphabet)
	{
		if (n == 0) {
			string s(1, alphabet[0]);
			return s;
		}

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
		map<string, int> dictionary;
		int dicIndex = 1;
		int codeWordForPrefix;
		string prefix = "";
		char currentChar;

		string code = "";

		dictionary.insert(pair<string, int>("", 0));
		for (int i = 0; i < txt.length(); i++)
		{
			if(dictionary.find(prefix + txt[i]) != dictionary.end())
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
					codeWordForPrefix = dictionary.find(prefix)->second;

				code += cw_encode(codeWordForPrefix, txt[i], A, E);
				
				dictionary.insert(pair<string, int>(prefix + txt[i], dicIndex));
				dicIndex++;
				prefix = "";
			}
		}
		return code;
	}

	int strtoint(string s, int  indice,  vector<char> alphabet)
	{
		int r = alphabet.size();
		int rPow = 1;
		int n = 0;

		int pos;

		for (int j = s.length()-1; j >= indice; j--)
		{
			pos = find(alphabet.begin(), alphabet.end(), s[j]) - alphabet.begin();
			n = n + (pos * rPow);
			rPow = rPow * r;
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
			y = code.substr(j, k);
			j += k;
			k = strtoint(y, 0, E) + 2;
		}
		ret.first = strtoint(y, 1,  E);
		ret.second = j + 1;
		return ret;
	}

	string decode(string code, vector<char> A, vector<char> E)
	{
		string txt = "";
		vector<pair<int, int> > dictionary;
		dictionary.push_back(pair<int, int>(0, 0));
		int i = 0;
		int n = code.length();

		pair<int, int> pL, kl;

		while(i < n) {
			pL = cw_decode(code.substr(i), E);
			i += pL.second;
			kl = cw_decode(code.substr(i), E);
			i += kl.second;

			txt = txt + (txt.substr(dictionary[pL.first].first, dictionary[pL.first].second - dictionary[pL.first].first) + A[kl.first]);
			dictionary.push_back(pair<int,int>(txt.length() - ((dictionary[pL.first].second - dictionary[pL.first].first) + 1), txt.length()));
		}

		return txt.substr(0, txt.length() - 1);
	}

};

int main() {
	LZ78 lz78;

	ifstream in;
	std::ostringstream contents;
	string contentsStr;
	in.open("proteins.10MB");
	contents.str("");
	contents << in.rdbuf();
	in.close();
	contentsStr = contents.str();

	vector<char> eAlphabet;
	eAlphabet.push_back('0');
	eAlphabet.push_back('1');

	vector<char> alphabet;
	string a = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#%&$\'()*+,-./:;<=>!?@[\\]^_`{|}~\n";	
	
	for (int i = 0; i < a.length(); i++) {
		alphabet.push_back(a[i]);
	}
	
	string code = lz78.encode(contentsStr, alphabet, eAlphabet);
	printf("Comprimiu\n");

	//--------------------------------------------------------------------------
	//Save file


	/*//write in a binary file
	ofstream outfile ("compressed2.bin", std::ios::binary);
	unsigned short len = code.length();
	unsigned short bytes[4];
	bytes[0] = (len) & 0xFF;
	bytes[1] = (len >> 8) & 0xFF;
	outfile.write((char*)bytes, 2);
	outfile.write(code.c_str(), 8*len);
	outfile.close();

	//unsigned long x = code.length();
	//bitset<code.length()> binario (code);
	//ofstream outfile ("compressed.bin", std::ios::binary);
	//unsigned long n = binario.to_ulong() ;
    //outfile.write( reinterpret_cast<const char*>(&n), sizeof(n) ) ;
    //outfile.close();
	
	//ofstream outfile2 ("compressed2.bin", std::ios::binary);
	//outfile2.write((char*) code.c_str(), code.size());
	//outfile2.close();	
	
	

	cout << "Tamanho do texto original " << contentsStr.length()<< endl;
 
	// read binary file
	unsigned short val;
	unsigned char bytess[2];
	ifstream file ("compressed2.bin", std::ifstream::binary);
	file.read((char*) bytess, 2);
	val = bytess[0] | (bytess[1] << 8);
	char* buffer = new char[val];
	file.read(buffer, val);
	string str(buffer, val);
	delete[] buffer;
	in.open("compressed2.bin", std::ios::binary);
	contents.str("");
	contents << in.rdbuf();
	in.close();
	contentsStr = contents.str();

	
	cout << "Eles tao iguais " << contentsStr.length() <<  "    " << code.length() << endl;
	*/

	printf("Descomprimindo\n");
	string text = lz78.decode(code, alphabet, eAlphabet);
	cout << "passou do decode" << endl;

	ofstream file2;
	file2.open("saida.txt");
	file2 << text;
	file2.close();

	if(text == contentsStr){
		printf("YES\n");
	}
	else{
		printf("NO\n");
	}


	// if(code.length() > contentsStr.length()) {
	// 	cout << "sim" << endl;
	// } else {
	// 	cout << "nao" << endl;
	// }

	// printf("%s\n", contentsStr.c_str());
	// ofstream out ("saida.txt", std::ofstream::binary);
	// out.write(contentsStr.c_str(), contentsStr.length());

	return 0;
}

