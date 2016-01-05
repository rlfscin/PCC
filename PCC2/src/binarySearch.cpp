using namespace std;

class binarySearch {
public:
	binarySearch() {

	}

	int findSucessor(string T, string P, vector<int> S) {
		// P <=m T[S1..]
		if (P <= T.substr(S[0]).substr(P.length())) {
			return 0;
		// P >=m T[Sn..]
		} else if (P >= T.substr(S[S.size()-1]).substr(P.length())) {
			return S.size();
		} else {
			int l = 0, r = S.size()-1;
			int h = 0;
			while (r - l > 1) {
				h = floor((l+r)/2);
				// P <=m T[Sh..]
				if (P <= T.substr(S[h]).substr(P.length())) {
					r = h;
				} else {
					l = h;
				}
			}
			return r;
		}
	}

	int findPredecessor(string T, string P, vector<int> S) {
		// T[Sn..] <=m P
		if (T.substr(S[S.size()-1].substr(P.length())) <= P) {
			return S.size();
		// T[S1..] <=m P
		} else if (T.substr(S[0]).substr(P.length()) <= P) {
			return 0;
		} else {
			int l = 0, r = S.size()-1;
			int h = 0;
			while(r - l > 1) {
				h = floor((l+r)/2);
				// T[Sh..] <=m P
				if (T.substr(S[h]).substr(P.length()) <= P) {
					l = h;
				} else {
					r = h;
				}
			}
			return l;
		}
	}

	
}