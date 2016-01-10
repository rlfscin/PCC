#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

using namespace std;

string txt;

struct x {
	int a, b, index;
};

static int cmp(const struct x & one, const struct x & another) {
	return one.a == another.a ? (one.b < another.b ? 1 : 0) : (one.a < another.a ? 1 : 0);
}

vector<int> index(string text) {
	txt = text;
	int n = text.length();
	vector<x> L(n);

	for (int i = 0; i < n; i++) {
		L[i].a = text[i] - 'a';
		L[i].b = (i + 1 < n) ? (text[i + 1] - 'a') : -1;
		L[i].index = i;
	}

	sort(L.begin(), L.end(), cmp);

	vector<int> ind(n);

	int nextIdx;
	for (int k = 4; k < 2 * n; k = k * 2) {
		int rank = 0;
		int previousRank = L[0].a;
		L[0].a = rank;
		ind[L[0].index] = 0;

		for (int i = 1; i < n; i++) {
			if (L[i].a == previousRank && L[i].b == L[i - 1].b) {
				previousRank = L[i].a;
				L[i].a = rank;
			}
			else {
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

		sort(L.begin(), L.end(), cmp);
	}

	vector<int> sarray(n);
	for (int i = 0; i < n; i++)
	{
		sarray[i] = L[i].index;
	}

	return sarray;
}

inline int lcp(string & x, int ix, string & y, int iy){
	int lx = x.length();
	int ly = y.length();
	int i = 0;

	while ((i + ix < lx) && (i + iy < ly) && (x[i + ix] == y[i + iy])){
		i += 1;
	}
	return i;
}

inline int lcp2(string & txt, int x, int y){
	int n = txt.length();
	int i = 0;

	while (((i + x) < n) && ((i + y) < n) && (txt[x + i] == txt[y + i])){
		i += 1;
	}
	return i;
}

int succ_llcp(string & text, string & pat, vector<int> & sarray, vector<int> & Llcp, vector<int> & Rlcp){
	int patlen = pat.length();
	int txtlen = text.length();

	int L = lcp(pat, 0, text, sarray[0]);
	int R = lcp(pat, 0, text, sarray[txtlen - 1]);

	if ((L == patlen) || (((sarray[0] + L) < txtlen) && (pat[L] < (text[sarray[0] + L])))){
		return 0;
	}

	if ((((sarray[txtlen - 1] + R) == txtlen) && (R < patlen)) || ((R < patlen) && ((sarray[txtlen - 1] + R) < txtlen) && (pat[R] > (text[sarray[txtlen - 1] + R])))){
		return txtlen;
	}

	int l = 0;
	int r = txtlen - 1;

	while ((r - l) > 1){
		int h = (l + r) / 2;

		if (L >= R){
			if (L < Llcp[h]){
				l = h;
			}
			else if (L == Llcp[h]){
				int H = L + lcp(pat, L, text, sarray[h] + L);
				if ((H == patlen) || ((H < (txtlen - sarray[h])) && (text[sarray[h] + H] > pat[H]))){
					r = h;
					R = H;
				}
				else{
					l = h;
					L = H;
				}
			}
			else{
				r = h;
				R = Llcp[h];
			}
		}
		else{
			if (R < Rlcp[h]){
				r = h;
			}
			else if (R == Rlcp[h]){
				int H = R + lcp(pat, R, text, sarray[h] + R);
				if ((H == patlen) || ((H < (txtlen - sarray[h])) && (text[sarray[h] + H] > pat[H]))){
					r = h;
					R = H;
				}
				else{
					l = h;
					L = H;
				}
			}
			else{
				l = h;
				L = Rlcp[h];
			}
		}
	}
	return r;
}

int pred_llcp(string & text, string & pat, vector<int> & sarray, vector<int> & Llcp, vector<int> & Rlcp){
	int patlen = pat.length();
	int txtlen = text.length();

	int L = lcp(pat, 0, text, sarray[0]);
	int R = lcp(pat, 0, text, sarray[txtlen - 1]);


	if ((R == patlen) || ((R + sarray[txtlen - 1]) == txtlen) || ((text[R + sarray[txtlen - 1]]) < pat[R])){
		return txtlen - 1;
	}

	if ((L < patlen) && ((L + sarray[0]) < txtlen) && (text[L + sarray[0]] > pat[L])){
		return -1;
	}

	int l = 0;
	int r = txtlen - 1;
	while ((r - l) > 1){
		int h = (l + r) / 2;
		if (L >= R){
			if (L < Llcp[h]){
				l = h;
			}
			else if (L == Llcp[h]){
				int H = L + lcp(pat, L, text, sarray[h] + L);
				if ((H == patlen) || (txtlen == (H + sarray[h])) || (text[sarray[h] + H] < pat[H])){
					l = h;
					L = H;
				}
				else{
					r = h;
					R = H;
				}
			}
			else{
				r = h;
				R = Llcp[h];
			}
		}
		else{
			if (R < Rlcp[h]){
				r = h;
			}
			else if (R == Rlcp[h]){
				int H = R + lcp(pat, R, text, sarray[h] + R);
				if ((H == patlen) || (txtlen == (H + sarray[h])) || (text[sarray[h] + H] < pat[H])){
					l = h;
					L = H;
				}
				else{
					r = h;
					R = H;
				}
			}
			else{
				l = h;
				L = Rlcp[h];
			}
		}
	}
	return l;
}

void compute_LRlcp(string & text, vector<int> & sarray, vector<int> & Llcp, vector<int> & Rlcp, int l, int r){
	if ((r - l) > 1){
		int h = (l + r) / 2;

		Llcp[h] = lcp2(text, sarray[l], sarray[h]);
		Rlcp[h] = lcp2(text, sarray[r], sarray[h]);

		compute_LRlcp(text, sarray, Llcp, Rlcp, l, h);
		compute_LRlcp(text, sarray, Llcp, Rlcp, h, r);
	}
}

inline vector<int> match(string & text, string & pat, vector<int> & sarray, vector<int> & Llcp, vector<int> & Rlcp){
	printf("trying to match against \'%s\'\n", pat.c_str());
	int r = pred_llcp(text, pat, sarray, Llcp, Rlcp);
	int l = succ_llcp(text, pat, sarray, Llcp, Rlcp);
	vector<int> v;
	for (int i = l; i < r + 1; i++){
		v.push_back(sarray[i]);
	}
	return v;
}

int main(){

	ifstream in;
	std::ostringstream contents;
	in.open("bible.txt");
	contents.str("");
	contents << in.rdbuf();
	in.close();
	txt = contents.str();

	int textlen = txt.length();
	vector<int> sarray = index(txt);

	vector<int> Llcp(textlen);
	vector<int> Rlcp(textlen);
	memset(&Llcp[0], -1, textlen*sizeof(Llcp[0]));
	memset(&Rlcp[0], -1, textlen*sizeof(Llcp[0]));

	printf("Compute iniciando\n");
	compute_LRlcp(txt, sarray, Llcp, Rlcp, 0, textlen - 1);
	printf("Compute encerrando\n");

	vector<string> patterns;
	patterns.push_back("Jesus");
	patterns.push_back("God");


	for (int i = 0; i < patterns.size(); i++){
		vector<int> v = match(txt, patterns[i], sarray, Llcp, Rlcp);
		printf("Numbers of match %lu\n", v.size());
	}


}