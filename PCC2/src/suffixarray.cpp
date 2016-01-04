#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

class suffixarray {
public:
	suffixarray() {

	}

	struct x {
		int a, b, index;
	};

	static int cmp(struct x one, struct x another) {
		return one.a == another.a ? (one.b < another.b ? 1 : 0) : (one.a < another.a ? 1 : 0);
	}

	vector<int> index(string text) {
		int n = text.length();
		vector<int> P(n);

		for(int i = 0; i < n; i++) {
			P[i] = text[i];
		}

		// int count = 1;
		int lg2 = ceil(log2(n));
		vector<x> L(n);


		for(int k = 1; k < lg2; k++) {
			for (int i = 0; i < n; i++) {
				L[i].a = P[i];
				L[i].b = i + 1 < n ? P[i + 1] : -1;
				L[i].index = i;
			}

			sort(L.begin(), L.end(), this->cmp);

			for (int i = 0; i < n; i++) {
				P[L[i].index] = i > 0 && L[i].a == L[i-1].b && L[i].b == L[i-1].b ? P[L[i-1].index] : i;
			}
			// count *= 2;
		}

		return P;
	}
};

// int main() {
// 	suffixarray teste;

// 	ifstream in;
// 	std::ostringstream contents;
// 	string contentsStr;
// 	in.open("dna.50MB");
// 	contents.str("");
// 	contents << in.rdbuf();
// 	in.close();
// 	contentsStr = contents.str();

// 	vector<int> a = teste.index(contentsStr);

	// ofstream outfile ("indices.idx", ios::binary);
	// outfile.write((char *) &a, sizeof(a));
	// outfile.close();

	// ofstream FILE("indices.idx", ios::out | ofstream::binary);
	// copy(a.begin(), a.end(), ostreambuf_iterator<char>(FILE));

	// vector<int> newVector;
	// ifstream INFILE("indices.idx", ios::out | ofstream::binary);
	// istreambuf_iterator<char> iter(INFILE);
	// copy(iter.begin(), iter.end(), back_inserter(newVector));

	// vector<int> toRestore;
	// ifstream infile("indices.idx", ios::binary);
	// infile.read((char *) &toRestore, sizeof(toRestore));
	// infile.close();

	// for (int i = 0; i < a.size(); i++)
	// {
	// 	cout << a[i] << endl;
	// }

	// return 0;
// }