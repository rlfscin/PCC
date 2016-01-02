#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

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

		int count = 1;
		int lg2 = ceil(log2(n));
		x L[n];
		cout << "morreu na linha acima" <<endl;


		for(int k = 1; k < lg2; k++) {
			for (int i = 0; i < n; i++) {
				L[i].a = P[i];
				L[i].b = i + count < n ? P[i + count] : -1;
				L[i].index = i;
			}

			sort(L, L + n, this->cmp);

			for (int i = 0; i < n; i++) {
				P[L[i].index] = i > 0 && L[i].a == L[i-1].b && L[i].b == L[i-1].b ? P[L[i-1].index] : i;
			}
		}

		return P;
	}
};

int main() {
	suffixarray teste;

	ifstream in;
	std::ostringstream contents;
	string contentsStr;
	in.open("textoparaindice.txt");
	contents.str("");
	contents << in.rdbuf();
	in.close();
	contentsStr = contents.str();

	vector<int> a = teste.index(contentsStr);

	for (int i = 0; i < a.size(); ++i)
	{
		cout << a[i] << " ";
	}

	cout << "\n";

	return 0;
}