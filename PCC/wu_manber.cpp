//
//  wu_manber.cpp
//  PCC
//
//  Created by Rubens Lopes de Farias Silva on 10/20/15.
//  Copyright (c) 2015 Rubens Lopes de Farias Silva. All rights reserved.
//

#include <string>
#include <vector>
#include <limits.h>

using namespace std;

vector<int> wu_manber(string t, string p, int k)
{
    vector<int> pos;
    int size = p.size();
    int mask['~']; // '~' is the last char on asc2 table;
    int r[k+1];
    int old;
    fill(r, r + k, ~1);
    fill(mask, mask + '~', ~0);
    for(int i = 0; i < size; i++){
        mask[p[i]] &= ~(1 << i);
    }
    for(int i = 0; i < t.size();i++){
        old = r[0];
        r[0] |= mask[t[i]];
        r[0] <<= 1;
        for(int j = 1; j+1 < k; j++){
            int temp = r[j];
            r[j] = (old & (r[j] | mask[t[i]])) << 1; // | is going to be similar to == with the mask
            old = temp;
        }
        if((r[k] & (1 << size)) == 0){ // Find a match
            pos.push_back(i - size + 1);
        }

    }
    return pos;
}