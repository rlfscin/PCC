#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm> 
#include <string>
#include <map>

using namespace std;

class Node{
public:
    string c;
    int freq;
    Node *left;
    Node *right;
    Node(string c,int freq,Node *left, Node *right){
        this->c = c;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};

vector<Node> makeNodes(string txt){
    int count[256];
    vector<Node> v;
    memset(count, -1, sizeof count);
    int m = txt.size();
    for(int i =0; i < m; i++){
        if(count[txt[i]] == -1){
            count[txt[i]] = v.size();
            Node temp(string(1, txt[i]), 1, NULL, NULL);
            v.push_back(temp);
        }
        else{
            v[count[txt[i]]].freq++;
        }
    }
    return v;
}

void huffEncode(string txt){
    int n = txt.size();
    
}


int main() {
    vector<Node> v = makeNodes("abracadabra");
    for(int i = 0;i<v.size();i++){
        cout << v[i].c << " - " << v[i].freq << endl;
    }
    return 0;
}