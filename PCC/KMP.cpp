//
//  KMP.cpp
//  PCC
//
//  Created by Rubens Lopes de Farias Silva on 10/20/15.
//  Copyright (c) 2015 Rubens Lopes de Farias Silva. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class KMP{
public:
    KMP(string w){
        KMPTable(w);
        this->w = w;
    }
    ~KMP(){
        delete table;
    }
    
    //To avoid delete and recreate
    void renew(string w){
        delete table;
        KMPTable(w);
        this->w = w;
    }
    
    vector<int> search(string text){
        int t = 0;
        int i = 0;
        vector<int> v;
        while(i + t < text.length()){
            if(w[i] == text[i + t]){
                if(i+1 == w.length()){
                    v.push_back(t);
                    if(i > 0){
                        t += i - table[i];
                        i = table[i];
                    }
                    else{
                        i = 0;
                        t++;
                    }
                }
                else{
                    i++;
                }
            }else{
                if(i > 0){
                    t += i - table[i];
                    i = table[i];
                }
                else{
                    i = 0;
                    t++;
                }
            }
        }
        return v;
    }
    
    
private:
    
    int *table;
    string w;
    
    void KMPTable(string w){
        int pos = 2;
        int i = 0;
        table = new int[w.length()];
        table[0] = -1;
        table[1] = 0;
        while(pos < w.length()){
            //match
            if(w[pos-1] == w[i]){
                //both walk
                i++;
                table[pos] = i;
                pos++;
            }
            else if(i == 0){
                //just walk
                table[pos] = 0;
                pos++;
            }
            else{
                //roll back
                i = table[i];
            }
        }
    }
};