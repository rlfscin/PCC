#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include "suffixarray.cpp"

using namespace std;

class binarySearch {
public:
	binarySearch() {

	}

	string getSuffix(string S, int size) {
		if (S.length() <= size) {
			return S;
		} else {
			return S.substr(0, size);
		}
	}
	
	int findSucessor(string T, string P, vector<int> S) {
		// P <=m T[S1..]
		if (strcmp(P.c_str(), getSuffix(T.substr(S[0]), P.length()).c_str()) < 0
			|| strcmp(P.c_str(), getSuffix(T.substr(S[0]), P.length()).c_str()) == 0) {
			return 0;			
		}
		// P >m T[Sn..]
		else if (strcmp(P.c_str(), getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str()) > 0) {
			return S.size();
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);				
				// P <=m T[Sh..]
				if (strcmp(P.c_str(), getSuffix(T.substr(S[h]), P.length()).c_str()) < 0
					|| strcmp(P.c_str(), getSuffix(T.substr(S[h]), P.length()).c_str()) == 0) {
					r = h;					
				}
				else {					
					l = h;
				}
			}
			return r;
		}
	}

	int findPredecessor(string T, string P, vector<int> S) {
		// T[Sn..] <=m P
		if (strcmp(getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str(), P.c_str()) < 0
			|| strcmp(getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str(), P.c_str()) == 0) {
			return S.size()-1;			
		}
		// T[S1..] >m P
		else if (strcmp(getSuffix(T.substr(S[0]), P.length()).c_str(), P.c_str()) > 0) {
			return -1;
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);
				// T[Sh..] <=m P
				if (strcmp(getSuffix(T.substr(S[h]), P.length()).c_str(), P.c_str()) < 0
					|| strcmp(getSuffix(T.substr(S[h]), P.length()).c_str(), P.c_str()) == 0) {
					l = h;
				}
				else {
					r = h;
				}
			}
			return l;
		}
	}

	vector<string> search(string T, string P, vector<int> S) {
		int predecessor = findPredecessor(T, P, S);
		int sucessor = findSucessor(T, P, S);		
		
		printf("predecessor: %d\nsucessor: %d\n", predecessor, sucessor);
		if ((sucessor == S.size()) || (predecessor == -1)
			||(P > T.substr(S[predecessor]) && P < T.substr(S[sucessor]))) {
			vector<string> matches(0);
			return matches;
		} else {
			vector<string> matches(predecessor - sucessor + 1);

			int j = 0;
			for (int i = sucessor; i <= predecessor; i++)
			{
				matches[j] = T.substr(S[i]);
				j++;
			}
			return matches;
		}		
	}
};

int main() {
	binarySearch bs;
	suffixarray sa;

	// ifstream in;
	// std::ostringstream contents;
	// string contentsStr;
	// in.open("meComprima_menor.txt", std::ios::in | std::ios::binary);
	// contents.str("");
	// contents << in.rdbuf();
	// in.close();
	// contentsStr = contents.str();

	string T = "Lorem ipsum";
	string P = "Lorem";

	vector<int> S = sa.index(T);

	// for (int i = 0; i < S.size(); i++)
	// {
	// 	printf("sufixo %d (%d): %s\n", i + 1, S[i], T.substr(S[i]).c_str());
	// }

	vector<string> matches = bs.search(T, P, S);
	// printf("qtd: %zu\n", matches.size());
	cout << matches.size() << endl;
	// for (int i = 0; i < matches.size(); i++)
	// {
	// 	printf("occ %d: %s$\n", i + 1, matches[i].c_str());
	// }
	
	return 0;
}