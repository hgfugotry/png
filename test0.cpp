#include<fstream>
#include<string>
#include"LPng.hpp"
#include"crc.hpp"
#include<iostream>

int p;
int offset;
unsigned char dataiii[10000000];
int dataNum=0;

void disposeParams(int argc,char* argv[]);
int main(int argc,char* argv[])
{
    using namespace std;
    unsigned char terri[4]={3,3,3,3};
    unsigned int i=crc32(terri,4);
    cout<<i<<endl;
    ofstream fout(argv[1],ios_base::in|ios_base::out|ios_base::binary);
    disposeParams(argc,argv);
    
    //写入要更改的数据
    fout.seekp(p+offset);//输出定位绝对地址
    fout.write((char*)dataiii,dataNum);//写入数据
    cout<<dec<<fout.tellp()<<endl;

    fout.flush();

    ifstream fin(argv[1],ios_base::in|ios_base::binary);
    //读取长度
    fin.seekg(p);
    unsigned char temp[4];
    fin.read((char*)temp,4);
    reverse(temp,4);
    unsigned int signStart=*((unsigned int*)temp);
    cout<<signStart<<endl;
    cout<<dec<<fin.tellg()<<endl;

    //读取整块数据，计算CRC--存储在temp0中
    fin.seekg(p+4);
    fin.read((char*)dataiii,signStart+4);
    for(int i=0;i<8;i++)
    {
        cout<<(int)dataiii[i]<<' ';
    }
    cout<<endl;
    unsigned int buf=crc32(dataiii,signStart+4);
    cout<<"buf:"<<buf<<endl;
    unsigned char* temp0=(unsigned char*)(&buf);
    reverse(temp0,4);
    cout<<dec<<fin.tellg()<<endl;

    //写入CRC
    fout.seekp(p+8+*((unsigned int*)temp));
    fout.write((char*)temp0,4);
    for(int i=0;i<4;i++)
        cout<<hex<<(int)temp0[i]<<' ';
    cout<<endl;
    std::cout<<dec<<fout.tellp()<<std::endl;
}

void disposeParams(int argc, char *argv[])
{
    using namespace std;
    cout<<"argc:"<<argc<<endl;
    p=atoi(argv[2]);
    cout<<"p:"<<p<<endl;
    offset=atoi(argv[3]);
    cout<<"offset:"<<offset<<endl;
    for(int i=4;i<argc;i++)
    {
        dataiii[i-4]=atoi(argv[i]);
        cout<<"data["<<i-4<<"]:"<<hex<<(int)dataiii[i-4]<<endl;
        ++dataNum;
    }
}
