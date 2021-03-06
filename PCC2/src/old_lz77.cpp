// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <math.h>
// #include <algorithm> 

using namespace std;

class lz77 {
public:
	lz77() {

	}

	string code_int(int n, vector<char> alphabet, int nbits) {
		string code = "";
		int r = alphabet.size();

		int bit = 0;
		while(n) {
			bit = n % r;
			code = alphabet[bit] + code;
			n = n/r;
		}

		string first = "";
		for(int i = 0; i < nbits - code.length(); i++) {
			first += alphabet[0];
		}

		return first + code;
	}

	int decode_int(string code, vector<char> alphabet, int nbits) {
		int n = 0;
		int r = alphabet.size();
		int rpow = 1;
		int p = code.length() - 1;
		char bit;

		while(p >= 0) {
			bit = code[p];
			n = n + (find(alphabet.begin(), alphabet.end(), bit) - alphabet.begin()) * rpow;
			p = p - 1;
			rpow = rpow * r;
		}

		return n;
	}

	vector<int> prefix_match(string w, int ls) {
		int lmax = 0;
		int p = 0;
		int m = w.length();
		int l;

		for(int s = 0; s < ls; s++) {
			l = 0;
			while((ls + l < m) && (w[s + l] == w[ls + l])) {
				l += 1;
			}

			if(l > lmax) {
				lmax = l;
				p = s;
			}

		}

		vector<int> pl;
		pl.push_back(p);
		pl.push_back(lmax);

		return pl;
	}

	string encode(string txt, int ls, int ll, vector<char> alphabet) {
		txt = txt + "$";
		string code = "";
		int r = alphabet.size();
		int pbits = ceil(log(ls) / log(r));
		int lbits = ceil(log(ll) / log(r));
		int n = txt.length();
		string w = "";

		for(int i = 0; i < ls; i++) {
			w = w + "_";
		}

		w = w + txt.substr(0, min(n, ll));
		int j = -1;

		vector<int> pl;
		char c;

		while(w.length() > ls) {
			pl = prefix_match(w, ls);

			c = txt[j + pl[1] + 1];
			code = code + code_int(pl[0], alphabet, pbits) + code_int(pl[1], alphabet, lbits) + c;

			if(j+ll+1 > txt.length()) {
				w = w.substr(pl[1]+1);	
			} else {
				w = w.substr(pl[1]+1) + txt.substr(j + ll + 1, (min(n, j + ll + pl[1] + 2) - (j + ll + 1)));
			}

			j += (pl[1] + 1);
		}

		return code;
	}

	string decode(string code, int ls, int ll, vector<char> alphabet) {
		int r = alphabet.size();
		int pbits = ceil(log(ls) / log(r));
		int lbits = ceil(log(ll) / log(r));
		int i = 0;
		int n = code.length();
		
		string txt = "";
		for(int i = 0; i < ls; i++) {
			txt += "_";
		}

		int j = 0;
		int p;
		int l;
		char c;
		int k;

		while(i < n) {
			p = decode_int(code.substr(i, pbits), alphabet, pbits);
			i += pbits;
			l = decode_int(code.substr(i, lbits), alphabet, lbits);
			i += lbits;
			c = code[i];
			i += 1;
			k = 0;

			while(k < l) {
				txt += txt[j + p + k];
				k += 1;
			}

			txt += c;
			j += (l + 1);
		}

		return txt.substr(ls);
	}

	void compressThisPlease(string text) {
		vector<char> alphabet;
		string a = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#%&$\'()*+,-./:;<=>?@[\\]^_`{|}~";

		for (int i = 0; i < a.length(); i++) {
			alphabet.push_back(a[i]);
		}

		string encoded = encode(text, 2048, 2048, alphabet);
		printf("Finished compressing.\n");
		
		printf("Writing file...\n");
		ofstream outfile ("compressed.pt", std::ofstream::binary);
		outfile.write(encoded.c_str(), encoded.length());
	}

	void uncompressThisPlease(string code) {
		vector<char> alphabet;
		string a = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#%&$\'()*+,-./:;<=>?@[\\]^_`{|}~";	
		
		for (int i = 0; i < a.length(); i++) {
			alphabet.push_back(a[i]);
		}

		string decoded = decode(code, 2048, 2048, alphabet);

		printf("Writing file...\n");
		ofstream outfile ("uncompressed.pt", std::ofstream::binary);
		outfile.write(decoded.c_str(), decoded.length());
	}
};

// int main() {
// 	// vector<char> alphabet;
// 	// alphabet.push_back('a');
// 	// alphabet.push_back('b');
// 	// alphabet.push_back('c');
// 	// alphabet.push_back('d');
// 	// alphabet.push_back('r');

// 	string abr = "abracadabra";
// 	string txt = "";
// 	for(int i = 0; i < 2000; i++) {
// 		txt = txt + abr;
// 	}

// 	// lz77 lz;
// 	// string code = lz.encode(txt, 16, 16, alphabet);

// 	// printf("code= %s\n", code.c_str());

// 	// string decoded = lz.decode(code, 16, 16, alphabet);

// 	// printf("decoded= %s\n", decoded.c_str());

// 	lz77 lz;
// 	lz.compressThisPlease(txt);

// 	return 0;
// }
