#include <vector>
#include <string>

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
		if (strcmp(P.c_str(), getSuffix(T.substr(S[0]), P.length()).c_str()) == -1
			|| strcmp(P.c_str(), getSuffix(T.substr(S[0]), P.length()).c_str()) == 0) {
			return 0;			
		}
		// P >=m T[Sn..]
		else if (strcmp(P.c_str(), getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str()) == 1) {
			return S.size()-1;
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);				
				// P <=m T[Sh..]
				if (strcmp(P.c_str(), getSuffix(T.substr(S[h]), P.length()).c_str()) == -1
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
		if (strcmp(getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str(), P.c_str()) == -1
			|| strcmp(getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str(), P.c_str()) == 0) {
			return S.size()-1;			
		}
		// T[S1..] >=m P
		else if (strcmp(getSuffix(T.substr(S[0]), P.length()).c_str(), P.c_str()) == 1) {
			return 0;
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);
				// T[Sh..] <=m P
				if (strcmp(getSuffix(T.substr(S[h]), P.length()).c_str(), P.c_str()) == -1
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
		if ((P < getSuffix(T.substr(S[predecessor]), P.length()) 
				&& P < getSuffix(T.substr(S[sucessor]), P.length()))
			|| (P > getSuffix(T.substr(S[predecessor]), P.length()) 
				&& P > getSuffix(T.substr(S[sucessor]), P.length()))){
			vector<string> matches(0);
			return matches;
		} else if (P <= T.substr(S[predecessor]) || P >= T.substr(S[sucessor])) {
			vector<string> matches(predecessor - sucessor + 1);

			int j = 0;
			for (int i = sucessor; i <= predecessor; i++)
			{
				matches[j] = T.substr(S[i]);
				j++;
			}
			return matches;
		} else {
			vector<string> matches(0);
			return matches;
		}		
	}
};

/*int main() {
	binarySearch bs;
	suffixarray sa;

	string T = "bobocel";
	string P = "l";

	vector<int> S = sa.index(T);

	for (int i = 0; i < S.size(); i++)
	{
		printf("sufixo %d (%d): %s\n", i + 1, S[i], T.substr(S[i]).c_str());
	}

	vector<string> matches = bs.search(T, P, S);
	printf("qtd: %d\n", matches.size());
	for (int i = 0; i < matches.size(); i++)
	{
		printf("occ %d: %s\n", i + 1, matches[i].c_str());
	}
	
	return 0;
}*/