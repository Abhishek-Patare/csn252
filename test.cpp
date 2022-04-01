//
// Created by abhishek on 28/03/22.
//
#include <bits/stdc++.h>
using namespace  std;




#define nl '\n';.
#include "lib.cpp"

int main(){
    //{1,0,1,0,1,1,1,1,0,0}
    vector<bool>f =hex_tobool("2BC",10) ;
    for(int i=0;i<10;++i)cout<<f[i];
    
}