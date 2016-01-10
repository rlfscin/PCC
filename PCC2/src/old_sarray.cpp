#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

using namespace std;

string txt;

inline int scmp(string & x, string & y, int yi, int m){
	int lx = x.length();
	int ly = y.length();
	int i = 0;
	while((i< lx) && (i + yi < ly) && (i < m) && (x[i] == y[i + yi])){
		i++;
	}
	if(i == m){
		return 0;
	}
	if(i == lx){
		if(i + yi == ly){
			return 0;
		}
		else {
			return -1;
		}
	}
	if(i + yi == ly){
		return 1;
	}
	if(x[i] < y[i + yi]){
		return -1;
	}
	else{
		return 1;
	}
}

inline bool gt(string & x, string & y, int yi, int m){
	return scmp(x, y, yi, m ) > 0;
}

inline bool geq(string & x, string & y, int yi, int m){
	return scmp(x, y, yi, m ) >= 0;
}

inline bool lt(string & x, string & y, int yi, int m){
	return scmp(x, y, yi, m ) < 0;
}

inline bool leq(string & x, string & y, int yi, int m){
	return scmp(x, y, yi, m ) <= 0;
}



int succ(string & text, string & pat, vector<int> & sarray){
	int patlen = pat.length();
	int txtlen = text.length();

	if (leq(pat, text, sarray[0], patlen)){
		return 0;
	}

	if (gt(pat, text, sarray[txtlen - 1], patlen)){
		return txtlen;
	}
	int l = 0;
	int r = txtlen - 1;

	while ((r - l) > 1){

		int h = (l + r) / 2;
		if (leq(pat, text, sarray[h], patlen)){
			r = h;
		}
		else{
			l = h;
		}
	}
	return r;
}


inline int pred(string & text, string & pat, vector<int> & sarray){
	int patlen = pat.length();
	int txtlen = text.length();

	if (geq(pat, text, sarray[txtlen - 1], patlen)){
		return txtlen - 1;
	}

	if (lt(pat, text, sarray[0], patlen)){
		return -1;
	}

	int l = 0;
	int r = txtlen - 1;

	while ((r - l) > 1){
		int h = (l + r) / 2;

		if (lt(pat, text, sarray[h], patlen)){
			r = h;
		}
		else{
			l = h;
		}
	}
	return l;
}


inline vector<int> match(string & text,string & pat, vector<int> & sarray){
	int r = pred(text, pat, sarray);
	int l = succ(text, pat, sarray);
	vector<int> v;
	for(int i = l; i < r + 1; i++){
		v.push_back(sarray[i]);
	}
	return v;
}

bool sorter1(const int & p1, const int & p2){
	return txt[p1] < txt[p2];
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
	vector<int> v;
	for(int i = 0; i < txtlen; i++){
		v.push_back(i);
	}
	

	sort(v.begin(), v.end(), sorter1);


	vector<int> p(txtlen);

	memset(&p[0], -1, txtlen * sizeof(int));
	int k = 0;

	for(int i = 0; i < txtlen; i++){
		if((i > 0) && (txt[v[i]] != txt[v[i - 1]])){
			k += 1;
		}

		p[v[i]] = k;
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

		int q = 0;

		for(int i = 0; i < txtlen; i ++){
			if ((i > 0) && ((r[i].first != r[i - 1].first) || (r[i].second.first != r[i - 1].second.first))){
				q += 1;
			}

			p[r[i].second.second] = q;
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
 	in.open("../testes/fib_s14930352");
 	contents.str("");
 	contents << in.rdbuf();
 	in.close();
 	txt = contents.str();

	// int textlen = txt.length();
	vector<int> sarray = build_sarray_smart(txt);

	// vector<string> patterns;
	// patterns.push_back("Jesus");
	
	// for(int i = 0; i < patterns.size(); i++){
	// 	vector<int> v = match(txt, patterns[i], sarray);
	// 	printf("Numbers of match %lu\n",v.size());
	// }
}
