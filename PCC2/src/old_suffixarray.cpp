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
	vector<int> llcp;
	vector<int> rlcp;
	suffixarray() {

	}
	struct x {
		int a, b, index;
	};

	static int cmp(struct x one, struct x another) {
		return one.a == another.a ? (one.b < another.b ? 1 : 0) : (one.a < another.a ? 1 : 0);
	}
	
	void initializeVectors(int size){
		llcp.resize(size);
		rlcp.resize(size);
	}
	
	vector<int> getLlcp(){
		return llcp;
	}
	
	vector<int> getRlcp(){
		return rlcp;
	}
	
	int lcp(string x, string y) {
		int i;
        for (i = 0; 
        	i < x.length() 
        	&& i < y.length() 
        	&& x[i] == y[i];
        	i++);
        return i;
    }
	
	vector<int> getSALCP(vector<int> sa, string T) {
		vector<int> salcp(sa.size()-1);
		for(int i = 0; i < salcp.size(); i++) {
			salcp[i] = lcp(T.substr(sa[i]), T.substr(sa[i+1]));
		}
		return salcp;
	}
	
	void buildLlcpRlcp(string T, vector<int> S, int lo, int hi){
	    int m = 0;
	    if (hi - lo == 1) {
	    	return;
	    }
	    if (hi - lo > 1) {
	    	m = floor((hi + lo) / 2);
	    }
	
	    llcp[m] = lcp(T.substr(S[lo]), T.substr(S[m]));
	    rlcp[m] = lcp(T.substr(S[m]), T.substr(S[hi]));
	    
	    buildLlcpRlcp(T, S, lo, m);
	    buildLlcpRlcp(T, S, m, hi);
	}

	vector<int> index(string text) {
		int n = text.length();
		vector<x> L(n);

		for (int i = 0; i < n; i++) {
			L[i].a = text[i] - 'a';
			L[i].b = (i + 1 < n) ? (text[i + 1] - 'a'): -1;
			L[i].index = i;
		}

		sort(L.begin(), L.end(), this->cmp);

		vector<int> ind(n);

		int nextIdx;
		for (int k = 4; k < 2*n; k = k*2) {
			int rank = 0;
			int previousRank = L[0].a;
			L[0].a = rank;
			ind[L[0].index] = 0;

			for (int i = 1; i < n; i++) {
				if (L[i].a == previousRank && L[i].b == L[i - 1].b) {
					previousRank = L[i].a;
					L[i].a = rank;
				} else {
					previousRank = L[i].a;
					L[i].a = ++rank;
				}
				ind[L[i].index] = i;
			}

			for (int i = 0; i < n; i++)
			{
				nextIdx = L[i].index + k / 2;
				L[i].b = (nextIdx < n) ? L[ind[nextIdx]].a : -1;
			}

			sort(L.begin(), L.end(), this->cmp);
		}

		vector<int> sarray(n);
		for (int i = 0; i < n; i++)
		{
			sarray[i] = L[i].index;
		}

		return sarray;
	}
};

// int main() {
// 	suffixarray sa;

// 	// ifstream in;
// 	// std::ostringstream contents;
// 	// string contentsStr;
// 	// in.open("dna.50MB");
// 	// contents.str("");
// 	// contents << in.rdbuf();
// 	// in.close();
// 	// contentsStr = contents.str();

// 	string T = "abbabab";
// 	vector<int> S = sa.index(T);

// 	// ofstream outfile ("indices.idx", ios::binary);
// 	// outfile.write((char *) &a, sizeof(a));
// 	// outfile.close();

// 	// ofstream FILE("indices.idx", ios::out | ofstream::binary);
// 	// copy(a.begin(), a.end(), ostreambuf_iterator<char>(FILE));

// 	// vector<int> newVector;
// 	// ifstream INFILE("indices.idx", ios::out | ofstream::binary);
// 	// istreambuf_iterator<char> iter(INFILE);
// 	// copy(iter.begin(), iter.end(), back_inserter(newVector));

// 	// vector<int> toRestore;
// 	// ifstream infile("indices.idx", ios::binary);
// 	// infile.read((char *) &toRestore, sizeof(toRestore));
// 	// infile.close();

// 	for (int i = 0; i < S.size(); i++)
// 	{
// 		// cout << a[i] << endl;
// 		printf("%d ", S[i]);
// 	}
// 	printf("\n");

// 	return 0;
// }
