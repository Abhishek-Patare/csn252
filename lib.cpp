#include <bits/stdc++.h>
using namespace std;

bool isBlank(char c)
{
    return (c == ' ') || (c == '\t');
}

bool isGarbage(string s)        ///check what is thia
{
    return (s[0] == '.');
}

string int_tohex(int x, int n)  //bytes = size of x in hex;
{
   char hex_string[n];
   sprintf(hex_string, "%X", x); //convert number to hex
   return hex_string;

}

int hex_toInt(string hex)
{
    return stoi(hex,0,16);
}

string bool_tohex(vector<bool>&flags,int dig){
    int x=0;
    for(int i=flags.size()-1,j=1;i>=0;--i,j=j<<1){
        x+= flags[i]?j:0;    
    }
    return int_tohex(x,dig);

}

string bool_tohex(vector<bool>ub,vector<bool>lb,int dig){
    int x=0;
    int j=1;
    for(int i=lb.size()-1;i>=0;i--,j=j<<1){
        x+= lb[i]?j:0;
    }
    for(int i=ub.size()-1;i>=0;i--,j=j<<1){
        x+= ub[i]?j:0;
    }
    return int_tohex(x,dig);

}

vector<bool> hex_tobool(string hex,int bits){
    int x = hex_toInt(hex);
    vector<bool>f(bits);
    for(int i=bits-1,j=1;i>=0;--i,j=j<<1){
        f[i] = ((x&j) !=0);
    }
    return f;
}

string format_to_size(string s,int n,char c){
    reverse(s.begin(),s.end());
    char output[n];
    for(int i=0;i<n;++i){
        if(i<s.length())output[i]=s[i];
        else output[i]=c;
    }
    reverse(output,output+n);
    return output;
}

string string_tohex(string s, int bytes)
{
    stringstream ss;
    for (const auto &item : s) {
        ss << hex << int(item);
    }
    string s2 = ss.str();
    return s2;
}


