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
		if (P <= getSuffix(T.substr(S[0]), P.length())) {
			return 0;
			
		}
		// P >=m T[Sn..]
		else if (P >= getSuffix(T.substr(S[S.size() - 1]), P.length())) {
			return S.size();
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);
				// P <=m T[Sh..]
				if (P <= getSuffix(T.substr(S[h]), P.length())) {
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
		if (getSuffix(T.substr(S[S.size() - 1]), P.length()) <= P) {
			return S.size();
			
		}
		// T[S1..] <=m P
		else if (getSuffix(T.substr(S[0]), P.length()) >= P) {
			return 0;
		}
		else {
			int l = 0, r = S.size() - 1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l + r) / 2);
				// T[Sh..] <=m P
				if (getSuffix(T.substr(S[h]), P.length()) <= P) {
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

int main() {
	binarySearch bs;


}