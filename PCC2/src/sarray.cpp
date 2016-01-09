#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

using namespace std;

inline int lcp(string x,string y){
	int lx = x.length();
	int ly = y.length();
	int i = 0;

	while ((i < lx) && (i < ly) && (x[i] == y[i])){
		i += 1;
	}
	return i;
}

inline int lcp2(string & txt, int x, int y){
	int n = txt.length();
	int i = 0;

	while ((i < n) && (txt[x + i] == txt[y + i])){
		i += 1;
	}
	return i;
}

inline int scmp(string & x, string & y, int m){
	int lx = x.length();
	int ly = y.length();
	int i = 0;
	while((i < lx) && (i < ly) && (i < m) && (x[i] == y[i])){
		i++;
	}
	if( i == m){
		return 0;
	}
	if(i == lx){
		if(i == ly){
			return 0;
		}
		else {
			return -1;
		}
	}
	if(i == ly){
		return 1;
	}
	if(x[i] < y[i]){
		return -1;
	}
	else{
		return 1;
	}
}

inline bool gt(string x, string y, int m){
	return scmp(x, y, m ) > 0;
}

inline bool geq(string x, string y, int m){
	return scmp(x, y, m) >= 0;
}

inline bool lt(string x,string  y,int m){
	return scmp(x, y, m) < 0;
}

inline bool leq(string x,string y,int m){
	return scmp(x, y, m) <= 0;
}

inline bool eq(string x,string y,int m){
	return scmp(x, y, m) == 0;
}


int succ(string & text, string & pat, vector<int> & sarray){
	int patlen = pat.length();
	int txtlen = text.length();

	if (leq(pat, text.substr(sarray[0]), patlen)){
		return 0;
	}

	if (gt(pat, text.substr(sarray[txtlen - 1]), patlen)){
		return txtlen;
	}
	int l = 0;
	int r = txtlen - 1;

	while ((r - l) > 1){
		int h = (l + r) / 2;
		if (leq(pat, text.substr(sarray[h]), patlen)){
			r = h;
		}
		else{
			l = h;
		}
	}
	return r;
}

int succ_llcp(string & text,string & pat,vector<int> & sarray,vector<int> & Llcp, vector<int> & Rlcp){
	int patlen = pat.length();
	int txtlen = text.length();

	int L = lcp(pat, text.substr(sarray[0]));
	int R = lcp(pat, text.substr(sarray[txtlen - 1]));

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
				int H = L + lcp(pat.substr(L), text.substr(sarray[h] + L));
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
				int H = R + lcp(pat.substr(R), text.substr(sarray[h] + R));
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

	int L = lcp(pat, text.substr(sarray[0]));
	int R = lcp(pat, text.substr(sarray[txtlen - 1]));
	

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
				int H = L + lcp(pat.substr(L), text.substr(sarray[h] + L));
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
				int H = R + lcp(pat.substr(R), text.substr(sarray[h] + R));
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


inline int pred(string & text, string & pat, vector<int> & sarray){
	int patlen = pat.length();
	int txtlen = text.length();

	if (geq(pat, text.substr(sarray[txtlen - 1]), patlen)){
		return txtlen - 1;
	}

	if (lt(pat, text.substr(sarray[0]), patlen)){
		return -1;
	}

	int l = 0;
	int r = txtlen - 1;

	while ((r - l) > 1){
		int h = (l + r) / 2;

		if (lt(pat, text.substr(sarray[h]), patlen)){
			r = h;
		}
		else{
			l = h;
		}
	}
	return l;
}



void compute_LRlcp(string & text,vector<int> & sarray, vector<int> & Llcp,vector<int> & Rlcp,int l,int r){
	if ((r - l) > 1){
		int h = (l + r) / 2;

		Llcp[h] = lcp2(text, sarray[l], sarray[h]);
		Rlcp[h] = lcp2(text, sarray[r], sarray[h]);

		compute_LRlcp(text, sarray, Llcp, Rlcp, l, h);
		compute_LRlcp(text, sarray, Llcp, Rlcp, h, r);
	}
}

inline vector<int> match(string & text,string & pat, vector<int> & sarray, vector<int> & Llcp, vector<int> & Rlcp){
	printf("trying to match against \'%s\'\n" , pat.c_str());
	int r = pred(text, pat, sarray);
	printf("predecessor (R): %d / %d (com lcp)\n" , r, pred_llcp(text, pat, sarray, Llcp, Rlcp));
	int l = succ(text, pat, sarray);
	printf("successor (L): %d / %d (com lcp)\n" , l, succ_llcp(text, pat, sarray, Llcp, Rlcp));
	vector<int> v;
	for(int i = l; i < r + 1; i++){
		v.push_back(sarray[i]);
	}
	return v;
}

bool sorter1(const pair<char, int> & p1, const pair<char, int> & p2){
	return p1.first < p2.first;
}

bool sorter2(const pair<int,pair<int,int> > & p1,const pair<int, pair<int,int> > & p2){
	if (p1.first == p2.first){
		if(p1.second.first == p2.second.first){
			return p1.second.second < p2.second.second;
		}
		return p1.second.first < p2.second.first;
	}
	return p1.first < p2.first;
}

vector<int> build_sarray_smart(string & txt){
	int txtlen = txt.length();
	int l = (int) ceil(log2(txtlen));
	vector<pair<char,int> > v;
	for(int i = 0; i < txtlen; i++){
		pair<char, int> p;
		p.first = txt[i];
		p.second = i;
		v.push_back(p);
	}

	sort(v.begin(), v.end(), sorter1);

	vector<int> p(txtlen);

	memset(&p[0], -1, txtlen * sizeof(int));
	int k = 0;

	for(int i = 0; i < txtlen; i++){
		if((i > 0) && (v[i].first != v[i - 1].first)){
			k += 1;
		}

		p[v[i].second] = k;
	}


	vector<pair<int,pair<int,int> > > r(txtlen);

	for(k = 0; k < l; k++){
		int j = (int) pow(2, k);
		for(int i = 0; i < txtlen; i++){
			pair<int,pair<int,int> > t;
			if ((i + j) < txtlen){
				t.first = p[i];
				t.second.first = p[i + j];
				t.second.second = i;
			}
			else{
				t.first = p[i];
				t.second.first = -1;
				t.second.second = i;
			}
			r[i] = t;
		}

		sort(r.begin(), r.end(), sorter2);

		k = 0;

		for(int i = 0; i < txtlen; i ++){
			if ((i > 0) && (r[i].first != r[i - 1].first) && (r[i].second.first != r[i - 1].second.first) && (r[i].second.second != r[i - 1].second.second)){
				k += 1;
			}

			p[r[i].second.second] = k;
		}

	}
	vector<int> ret;

	for(int i = 0; i < r.size(); i++){
		ret.push_back(r[i].second.second);
	}
	return ret;
}

int main(){

	ifstream in;
 	std::ostringstream contents;
 	string txt = "Textando o codigo\n";
 	in.open("bible.txt");
 	contents.str("");
 	contents << in.rdbuf();
 	in.close();
 	txt = contents.str();


	int textlen = txt.length();
	vector<int> sarray = build_sarray_smart(txt);

	vector<int> Llcp(textlen);
	vector<int> Rlcp(textlen);
	memset(&Llcp[0], -1, textlen*sizeof(Llcp[0]));
	memset(&Rlcp[0], -1, textlen*sizeof(Llcp[0]));

	printf("Compute iniciando\n");
	compute_LRlcp(txt, sarray, Llcp, Rlcp, 0, textlen - 1);
	printf("Compute encerrando\n");

	vector<string> patterns;
	patterns.push_back("God");
	//patterns.push_back("created");


	
	for(int i = 0; i < patterns.size(); i++){
		vector<int> v = match(txt, patterns[i], sarray, Llcp, Rlcp);
		printf("Numbers of match %lu\n",v.size());
		printf("[");
		for(int j = 0; j < v.size(); j++){
			printf("%c%c%c%c%c%c, ",txt[v[j] -5 ],txt[v[j] -4 ], txt[v[j]-3],txt[v[j]-2],txt[v[j]-1],txt[v[j]]);
		}
		printf("]\n");
	}
		

}
