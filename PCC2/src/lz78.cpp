#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unordered_map>
#include <map>

using namespace std;

class LZ78 {
public:
	LZ78(){}

	inline map<int, pair<int, char> > encode(string & text) {
		unordered_map<string, int> dictionary;
		string prefix = "";
		int dictionaryIndex = 1;

		int n = text.length();
		int codeWordForPrefix = 0;
		char c;

		map<int, pair<int, char> > code;
		int aux = 0;

		for(int i = 0; i < n; i++) {
			c = text[i];

			if(dictionary.find(prefix + c) != dictionary.end()) {
				prefix = prefix + text[i];
			} else {
				if(prefix == "") {
					codeWordForPrefix = 0;
				} else {
					codeWordForPrefix = dictionary.find(prefix)->second;
				}

				code.insert(pair<int, pair<int, char> >(aux, pair<int, char>(codeWordForPrefix, c)));
				aux++;

				dictionary.insert(pair<string, int>(prefix + c, dictionaryIndex));
				dictionaryIndex++;

				prefix = "";
			}
		}

		if(prefix != "") {
			codeWordForPrefix = dictionary.find(prefix)->second;
			char c = ' ';
			code.insert(pair<int, pair<int, char> >(aux, pair<int, char>(codeWordForPrefix, c)));
		}

		return code;
	}

	inline string decode(map<int, pair<int, char> > & code) {
		unordered_map<int, string> dictionary;
		map<int, pair<int, char> >::iterator it;
		int dictionaryIndex = 1;
		int codeWord;
		char c;
		string s;
		string text = "";

		for(it = code.begin(); it != code.end(); it++) {
			codeWord = it->second.first;
			c = it->second.second;

			if(codeWord == 0) {
				s = "";
			} else {
				s = dictionary.find(codeWord)->second;
			}

			text += s + c;
			dictionary.insert(pair<int, string>(dictionaryIndex, s + c));
			dictionaryIndex++;
		}

		return text;
	}

};

int main() {
	LZ78 lz78;

	// ifstream in;
	// std::ostringstream contents;
	// string contentsStr;
	// in.open("../testes/dna.50MB");
	// contents.str("");
	// contents << in.rdbuf();
	// in.close();
	// contentsStr = contents.str();

	// map<int, pair<int, char> > code = lz78.encode(contentsStr);

	// ofstream outfile ("../files/dna50.pt", ios::out | ios::binary);
	// map<int, pair<int, char> >::iterator it;

	// int qtd = code.size();
	// outfile.write((const char*) &qtd, sizeof(int));

	// for (it = code.begin(); it != code.end(); ++it)
	// {
	// 	outfile.write((const char*)&it->second.first, sizeof(int));
	// 	outfile.write((const char*)&it->second.second, sizeof(char));
	// }
	// outfile.close();



	streampos fileSize;
	ifstream file("../files/dna50.pt", ios::in | ios::binary);

	int qtd;
	file.read((char *) &qtd, sizeof(int));

	map<int, pair<int, char> > code;

	int inteiro = 0;;
	char c = ' ';
	int aux = 0;

	for (int i = 0; i < qtd; i++)
	{
		file.read((char *) &inteiro, sizeof(int));
		file.read((char *) &c, sizeof(char));

		code.insert(pair<int, pair<int, char> >(aux, pair<int, char>(inteiro, c)));
		aux++;
	}
	file.close();

	string text = lz78.decode(code);

	ofstream out ("../files/dna.50MB", ios::out | ios::binary);
	for (int i = 0; i < text.length(); i++)
	{
		out.write((char*) &text[i], sizeof(char));
	}
	out.close();

	return 0;
}