#include<fstream>
#include<string>
#include"LPng.hpp"
int main(int argc,char* argv[])
{
    for(int i=1;i<argc;i++)
    {
        std::string temp=argv[i];
        temp+=".report";
        reportPngDataBlock(argv[i],temp.c_str());
    };
}