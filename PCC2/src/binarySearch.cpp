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
		if (strcmp(P.c_str(), getSuffix(T.substr(S[0]), P.length()).c_str()) == -1) {
			return 0;			
		}
		// P >=m T[Sn..]
		else if (strcmp(P.c_str(), getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str()) == 1) {
			return S.size();
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);
				// P <=m T[Sh..]
				if (strcmp(P.c_str(), getSuffix(T.substr(S[h]), P.length()).c_str()) == -1) {
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
		if (strcmp(getSuffix(T.substr(S[S.size() - 1]), P.length()).c_str(), P.c_str()) == -1) {
			return S.size();			
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
				if (strcmp(getSuffix(T.substr(S[h]), P.length()).c_str(), P.c_str()) == -1) {
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
		vector<string> matches(predecessor - sucessor + 1);
		int j = 0;
		for (int i = sucessor; i <= predecessor; i++)
		{
			matches[j] = T.substr(S[i]);
			j++;			
		}
		printf("yam");
		return matches;
	}
};