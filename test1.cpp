#include<fstream>
#include<string>
#include"LPng.hpp"
#include"crc.hpp"
#include<iostream>

int main()
{
    using namespace std;
    unsigned char d[]={0x49,0x48,0x44,0x52,0x0,0x0,0x7,0x80,0x0,0x0,0x4,0x38,0x8,0x2,0x0,0x0,0x0};
    unsigned int a=crc32(d,sizeof(d));
    cout<<sizeof(d)<<endl;
    unsigned char* b=(unsigned char*)(&a);
    reverse(b,4);
    ofstream fout("1.txt",ios_base::in|ios_base::out|ios_base::binary);
    fout.seekp(0);
    fout.write((char*)b,4);
}