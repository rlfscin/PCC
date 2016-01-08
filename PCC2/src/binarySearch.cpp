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
	
	int lcp(string x, string y) {
		int i;
        for (i = 0; 
        	i < x.length() 
        	&& i < y.length() 
        	&& x[i] == y[i];
        	i++);
        return i;
    }
	
	int findSucessor(string T, string P, vector<int> S, vector<int> llcp, vector<int> rlcp) {
		int L = lcp(T.substr(S[0]), P);
		int R = lcp(T.substr(S[S.size()-1]), P);
		int n = P.length();

		if(L == n || (S[0] + L < S.size() && P[L] < T[S[0] + L])) {
			return 0;
		} 
		if ((S[S.size() - 1] + R == S.size() && R < n) || (R < n && S[S.size() - 1] + R < S.size() && P[R] > T[S[S.size() - 1] + R])) {
			return S.size();
		} 
		int l = 0, r = S.size() - 1;
		int h = 0, H = 0;
		while (r - l > 1) {
			h = floor((l + r) / 2);
			if (L >= R) {
				if (L < llcp[h]) {
					l = h;
				} else if (L == llcp[h]) {
					H = L + lcp(P.substr(L), T.substr(S[h] + L));
					if (H == n || (H < (S.size() - S[h]) && T[S[h] + H] > P[H])) {
						r = h;
						R = H;
					} else {
						l = h;
						L = H;
					}
				} else {
					r = h;
					R = llcp[h];
				}
			} else {
				if (R < rlcp[h]) {
					r = h;
				} else if (R == rlcp[h]) {
					H = R + lcp(P.substr(R), T.substr(S[h] + R));
					if (H == n || (H < (S.size() - S[h]) && T[S[h] + H] > P[h])){
						r = h;
						R = H;
					} else {
						l = h;
						L = H;
					}
				} else {
					l = h;
					L = rlcp[h];
				}
			}
		}
		return r;		
	}

	int findPredecessor(string T, string P, vector<int> S, vector<int> llcp, vector<int> rlcp) {
		int L = lcp(T.substr(S[0]), P);
		int R = lcp(T.substr(S[S.size()-1]), P);

		int n = P.length();
		if(R == n || R + S[S.size() - 1] == S.size() || T[R + S[S.size() - 1]] < P[R]) {
			return S.size()-1;
		} 
		if (L < n && L + S[0] < S.size() && T[L + S[0]] > P[L]) {
			return -1;
		} 
		int l = 0, r = S.size() - 1;
		int h = 0, H = 0;
		while (r - l > 1) {
			h = floor((l + r) / 2);
			if (L >= R) {
				if (L < llcp[h]) {
					l = h;
				} else if (L == llcp[h]) {
					H = L + lcp(P.substr(L), T.substr(S[h] + L));
					if (H == n || S.size() == H + S[h] || T[S[h] + H] < P[H]) {
						l = h;
						L = H;
					} else {
						r = h;
						R = H;
					}
				} else {
					r = h;
					R = llcp[h];
				}
			} else {
				if (R < rlcp[h]) {
					H = R + lcp(P.substr(R), T.substr(S[h] + R));
					if (H == n || S.size() == H + S[h] || T[S[h] + H] < P[H]) {
						l = h;
						L = H;
					} else {
						r = h;
						R = H;
					}
				} else {
					l = h;
					L = rlcp[h];
				}
			}
		}
		return l;
	}

	vector<string> search(string T, string P, vector<int> S, vector<int> llcp, vector<int> rlcp) {
		int predecessor = findPredecessor(T, P, S, llcp, rlcp);
		int sucessor = findSucessor(T, P, S, llcp, rlcp);		
		
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

// int main() {
// 	binarySearch bs;
// 	suffixarray sa;

// 	// ifstream in;
// 	// std::ostringstream contents;
// 	// string contentsStr;
// 	// in.open("meComprima_menor.txt", std::ios::in | std::ios::binary);
// 	// contents.str("");
// 	// contents << in.rdbuf();
// 	// in.close();
// 	// contentsStr = contents.str();

// 	string T = "abbabab";
// 	string P = "c";

// 	vector<int> S = sa.index(T);
// 	// vector<int> salcp = sa.getSALCP(S, T);
	
// 	vector<int> llcp(S.size()-1);
// 	vector<int> rlcp(S.size()-1);
	
// 	sa.initializeVectors(S.size()-1);
	
// 	int hi = S.size()-1;
	
// 	sa.buildLlcpRlcp(T, S, 0, hi);

// 	for (int i = 0; i < S.size(); i++)
// 	{
// 		printf("sufixo %d (%d): %s\n", i, S[i], T.substr(S[i]).c_str());
// 	}
// 	for (int i = 0; i < sa.getLlcp().size(); i++) {
// 		printf("llcp[%d]: %d\n",i,sa.getLlcp()[i]);
// 	}
// 	for (int i = 0; i < sa.getRlcp().size(); i++) {
// 		printf("rlcp[%d]: %d\n",i,sa.getRlcp()[i]);
// 	}
	
// 	vector<string> matches = bs.search(T, P, S, sa.getLlcp(), sa.getRlcp());
// 	// printf("qtd: %zu\n", matches.size());
// 	cout << matches.size() << endl;
// 	for (int i = 0; i < matches.size(); i++)
// 	{
// 		printf("occ %d: %s$\n", i + 1, matches[i].c_str());
// 	}
	
// 	return 0;
// }