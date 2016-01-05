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

	vector<short> encode(string text) {

		unordered_map<string, short> dictionary;

		string a;
		for(short i = 0; i < 256; i++) {
			a = (char) i;
			dictionary.insert(pair<string, short>(a, i));
		}

		string prefix = "";
		prefix += text[0];

		short codeword = 256;
		int n = text.length();
		char c;

		vector<short> code;

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
					for(short i = 0; i < 256; i++) {
						a = (char) i;
						dictionary.insert(pair<string, short>(a, i));
					}

					// cout << "clear" << endl;
				} else {
					dictionary.insert(pair<string, short>(prefix + c, codeword));
					codeword++;
					prefix = c;
				}

			}
		}

		// code.push_back(this->codeForWord(prefix, dictionary));
		code.push_back(dictionary.find(prefix)->second);

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

	string decode(vector<short> code) {
		unordered_map<short, string> dictionary;
		unordered_map<short, string>::const_iterator it;

		string a;
		for(short i = 0; i < 256; i++) {
			a = (char) i;
			dictionary.insert(pair<short, string>(i, a));
		}

		short previousCodeWord = code[0];
		string text = "";
		text += (char) previousCodeWord;

		char c = code[0];
		short codeword = 256;

		int n = code.size();

		short curretCW;
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

				for(short i = 0; i < 256; i++) {
					a = (char) i;
					dictionary.insert(pair<int, string>(i, a));
				}

				// cout << "clear" << endl;
			} else {
				dictionary.insert(pair<short, string>(codeword, aux));
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


int main() {
	LZW lzw;

	ifstream in;
	std::ostringstream contents;
	string contentsStr;
	in.open("../../dna.50MB", std::ios::in | std::ios::binary);
	contents.str("");
	contents << in.rdbuf();
	in.close();
	contentsStr = contents.str();

	vector<short> a = lzw.encode(contentsStr);

	ofstream outfile ("../files/magica.pt", ios::out | ios::binary);
	for (int i = 0; i < a.size(); ++i)
	{
		outfile.write((const char*)&a[i], sizeof(short));
	}
	outfile.close();


	streampos fileSize;
	ifstream file("../files/magica.pt", ios::in | ios::binary);

	// get its size:
	file.seekg(0, ios::end);
	fileSize = file.tellg();
	file.seekg(0, ios::beg);

	int tamanho = fileSize/sizeof(short);

	vector<short> b(tamanho);
	for (int i = 0; i < tamanho; i++)
	{
		file.read((char *) &b[i], sizeof(short));
	}
	file.close();

	string c = lzw.decode(b);

	ofstream out ("../files/dna.50MB", ios::out | ios::binary);
	for (int i = 0; i < c.length(); i++)
	{
		out.write((char*) &c[i], sizeof(char));
	}
	out.close();

	return 0;
}