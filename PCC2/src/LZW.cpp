#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class LZW {
public:
	LZW() {

	}

	vector<int> encode(string text) {

		unordered_map<string, int> dictionary;

		string a;
		for(int i = 0; i < 256; i++) {
			a = (char) i;
			dictionary.insert(pair<string, int>(a, i));
		}

		string prefix = "";
		prefix += text[0];

		int codeword = 256;
		int n = text.length();
		char c;

		vector<int> code;

		for(int i = 1; i < n; i++) {
			c = text[i];

			if(dictionary.find(prefix + c) != dictionary.end()) {
				prefix = prefix + c;
			} else {
				// code.push_back(this->codeForWord(prefix, dictionary));
				code.push_back(dictionary.find(prefix)->second);

				if(dictionary.size() > 7934) {
					dictionary.clear();
					codeword = 256;
					prefix = text[i];

					string a;
					for(int i = 0; i < 256; i++) {
						a = (char) i;
						dictionary.insert(pair<string, int>(a, i));
					}

					// cout << "clear" << endl;
				} else {
					dictionary.insert(pair<string, int>(prefix + c, codeword));
					codeword++;
					prefix = c;
				}

			}
		}

		code.push_back(this->codeForWord(prefix, dictionary));

		cout << dictionary.size() << endl;

		return code;
	}

	int codeForWord(string s, unordered_map<string, int> dic) {
		unordered_map<string, int>::const_iterator it = dic.find(s);

		if(it != dic.end()) {
			return it->second;
		} else {
			return s[0];
		}
	}

	string decode(vector<int> code) {
		unordered_map<int, string> dictionary;
		unordered_map<int, string>::const_iterator it;

		string a;
		for(int i = 0; i < 256; i++) {
			a = (char) i;
			dictionary.insert(pair<int, string>(i, a));
		}

		int previousCodeWord = code[0];
		string text = "";
		text += (char) previousCodeWord;

		char c = code[0];
		int codeword = 256;

		int n = code.size();

		int curretCW;
		string s;
		string aux;
		string x;
		for(int i = 1; i < n; i++) {
			curretCW = code[i];

			it = dictionary.find(curretCW);
			if(it != dictionary.end()) {
				s = it->second;

				text += s;
				c = s[0];
				// aux = this->wordForCode(previousCodeWord, dictionary);
				aux = dictionary.find(previousCodeWord)->second;
				aux += c;
			} else {
				// x = this->wordForCode(previousCodeWord, dictionary);
				x = dictionary.find(previousCodeWord)->second;
				s = x;
				s += c;

				text += s;
				c = s[0];
				aux = x;
				aux += c;
			}

			// text += s;
			// c = s[0];
			// aux = this->wordForCode(previousCodeWord, dictionary);
			// aux += c;

			if(dictionary.size() > 7934) {
				dictionary.clear();
				previousCodeWord = code[i];
				codeword = 256;

				for(int i = 0; i < 256; i++) {
					a = (char) i;
					dictionary.insert(pair<int, string>(i, a));
				}

				// cout << "clear" << endl;
			} else {
				dictionary.insert(pair<int, string>(codeword, aux));
				previousCodeWord = curretCW;
				codeword++;
			}

		}

		cout << dictionary.size() << endl;

		return text;
	}

	// string wordForCode(int cw, unordered_map<int, string> dic) {
	// 	unordered_map<int, string>::const_iterator it = dic.find(cw);

	// 	if(it != dic.end()) {
	// 		return it->second;
	// 	} else {
	// 		string a;
	// 		a = (char) cw;
	// 		return a;
	// 	}
	// }
};

typedef unsigned char BYTE;

int main() {
	// string entrada = "";
	// string x = "fhadosdalsdhajskdaskjda974378924ndadnabkdabdabiuda asd8293 ";

	// for (int i = 0; i < 100; ++i)
	// {
	// 	entrada += x;
	// }

	ifstream in;
	std::ostringstream contents;
	string contentsStr;
	in.open("../../dna.50MB", std::ios::in | std::ios::binary);
	contents.str("");
	contents << in.rdbuf();
	in.close();
	contentsStr = contents.str();

	LZW lzw;
	vector<int> a = lzw.encode(contentsStr);
	// ate a linha acima, aparentemente esta funcionando.
	// o texto esta sendo comprimido para o vecotr<int>

	// essa escrita pode estar errada. nao tenho certeza
	ofstream outfile ("magica.pt", ios::out | ios::binary);
	for (int i = 0; i < a.size(); ++i)
	{
		outfile.write((const char*)&a[i], sizeof(int));
	}
	outfile.close();


	// streampos fileSize;
 //    ifstream file("magica.pt", std::ios::binary);

 //    // get its size:
 //    file.seekg(0, ios::end);
 //    fileSize = file.tellg();
 //    file.seekg(0, ios::beg);

 //    // read the data:
 //    std::vector<BYTE> fileData(fileSize);
 //    file.read((char*) &fileData[0], fileSize);

	vector<int> b(a.size());
	ifstream file("magica.pt", ios::in | ios::binary);
	for (int i = 0; i < a.size(); i++)
	{
		file.read((char *) &b[i], sizeof(int));
	}
	file.close();

    // vector<int> teste;
    // for (int i = 0; i < fileData.size(); ++i)
    // {
    // 	teste[i] = fileData[i];
    // }

	string c = lzw.decode(b);
	cout << c << endl;
	// if(c == contentsStr) {
	// 	cout << "boa" << endl;
	// } else {
	// 	cout << "reprovasse" << endl;
	// }

	// cout << b << endl;

	return 0;
}