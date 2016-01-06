#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int m = -2147483648;

vector<int> convertToBits(string s){
	vector<int> v;
	v.push_back(s.length());
	int c = 0;
	if(s[0] == '1'){
		c = (c << 1) | 1;
	}
	else{
		c = c << 1;
	}
	for(int i = 1; i < s.length(); i++){
		if((i % 32) == 0){
			v.push_back(c);
			c = 0;
		}
		if(s[i] == '1'){
			c = (c << 1) | 1;
		}
		else{
			c = c << 1;
		}
	}
	v.push_back(c);
	return v;
}

vector<char> convertToString(vector<int> v){
	vector<char> t;
	int n = v[0] % 32;
	for(int i = 1; i < v.size()-1; i++){
		int c = v[i];
		for(int j = 0; j < 32; j++){
			if((c & m) != 0){
				t.push_back('1');
			}
			else{
				t.push_back('0');
			}
			c = c << 1;
		}
	}
	int c = v[v.size()-1];
	if(n == 0) n = 32;
	c = c << (32 - n);
	for(int j = 0; j < n; j++){
		if((c & m) != 0){
			t.push_back('1');
		}
		else{
			t.push_back('0');
		}
		c = c << 1;
	}
	return t;
}

void saveInFile(string fileName, string v){
	vector<int> t = convertToBits(v);
	ofstream out(fileName.c_str(), ios::out | ios::binary);
	cout << t.size() << endl;
	for (int i = 0; i < t.size(); i++) { 
		out.write((const char *)&t[i], sizeof(int)); 
	}
    out.close();
}

vector<char> readFromFile(string fileName){
	ifstream file(fileName.c_str(), ios::in | ios::binary);
	int c;
	file.seekg(0, ios::end);
	int fileSize = file.tellg();
	file.seekg(0, ios::beg);

	int tamanho = fileSize/sizeof(int);
	vector<int> t(tamanho);
	for (int i = 0; i < tamanho; i++)
	{
		file.read((char *) &t[i], sizeof(int));
	}
	file.close();
	
	return convertToString(t);
}
/*
int main(){
	string s = "101010101101010110111100010101010101010101011111010101010101001010101010100111110101010100101011010101010011100100010010101010101010100101110100101010100010111010101";
	vector<int> t = convertToBits(s);
	for(int i = 0; i < t.size(); i++){
		cout << t[i] << endl;
	}
	vector<char> r = convertToString(t);
	for(int i = 0; i < r.size(); i++){
		cout << r[i];
	}
	cout << endl;
	cout << s.c_str() <<  endl;
	saveInFile("Rubens.inf",s);
	vector<char> v = readFromFile("Rubens.inf");
	for(int i = 0; i < r.size(); i++){
		cout << r[i];
	}
	cout << endl;

	return 0;
}*/