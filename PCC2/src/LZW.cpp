#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class LZW {
public:
	LZW() {

	}

	vector<int> encode(string text) {
		string prefix = "";
		prefix += text[0];

		int codeword = 256;
		int n = text.length();
		char c;
		map<string, int> dictionary;

		vector<int> code;

		for(int i = 1; i < n; i++) {
			c = text[i];

			if(dictionary.find(prefix + c) != dictionary.end()) {
				prefix = prefix + c;
			} else {
				code.push_back(this->code(prefix, dictionary));
				dictionary.insert(pair<string, int>(prefix + c, codeword));
				codeword++;
				prefix = c;
			}
		}

		code.push_back(this->code(prefix, dictionary));

		return code;
	}

	int code(string s, map<string, int> dic) {
		map<string, int>::iterator it = dic.find(s);

		if(it != dic.end()) {
			return it->second;
		} else {
			return s[0];
		}
	}
};

int main() {
	LZW lzw;
	vector<int> a = lzw.encode("BABAABAAA");

	for (int i = 0; i < a.size(); ++i)
	{
		cout << a[i] << endl;
	}

	return 0;
}