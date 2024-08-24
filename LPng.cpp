#include"LPng.hpp"
#include"crc.hpp"
#include<fstream>
#include<iomanip>
#include<ctime>
#include<iostream>
static void printArray(unsigned char* array,unsigned int len,std::ofstream& fout);
void reportPngDataBlock(const char *file,const char* output)
{
    static const unsigned char IHDR[4]={73,72,68,82};//文件头数据块
    static const unsigned char cHRM[4]={99,72,82,77};//基色和白色点数据块
    static const unsigned char gAMA[4]={103,65,77,65};//图像gamma数据块
    static const unsigned char iCCP[4]={105,67,67,80};//嵌入ICC Profile
    static const unsigned char sBIT[4]={115,66,73,84};//样本有效位数据块
    static const unsigned char sRGB[4]={115,82,71,66};//标准RGB彩色空间
    static const unsigned char PLTE[4]={80,76,84,69};//调色板数据块
    static const unsigned char bKGD[4]={98,75,71,68};//背景颜色数据块
    static const unsigned char hIST[4]={104,73,83,84};//图像直方图数据块
    static const unsigned char tRNS[4]={116,82,78,83};//图像透明数据块
    static const unsigned char oFFs[4]={111,85,85,115};//（专用公共数据块）
    static const unsigned char pHYs[4]={112,72,89,115};//物理像素尺寸数据块
    static const unsigned char sPLT[4]={115,80,76,84};//建议调色板
    static const unsigned char sCAL[4]={115,67,65,73};//（专用公共数据块）
    static const unsigned char IDAT[4]={73,68,65,84};//图像数据块
    static const unsigned char tIME[4]={116,73,77,69};//图像最后修改时间数据块
    static const unsigned char tEXt[4]={105,84,88,116};//文本信息数据块
    static const unsigned char iTXt[4]={105,84,88,116};//国际文本数据块
    static const unsigned char zTXt[4]={122,84,88,116};//压缩文本数据块
    static const unsigned char fRAc[4]={102,82,65,99};//（专用公共数据块）
    static const unsigned char gIFg[4]={103,73,85,103};//（专用公共数据块）
    static const unsigned char gIFt[4]={103,73,85,116};//（专用公共数据块）
    static const unsigned char gIFx[4]={103,73,85,120};//（专用公共数据块）
    static const unsigned char IEND[4]={73,69,78,68};//图像结束数据

    int cIHDR=0;
    int ccHRM=0;
    int cgAMA=0;
    int ciCCP=0;
    int csBIT=0;
    int csRGB=0;
    int cPLTE=0;
    int cbKGD=0;
    int chIST=0;
    int ctRNS=0;
    int coFFs=0;
    int cpHYs=0;
    int csPLT=0;
    int csCAL=0;
    int cIDAT=0;
    int ctIME=0;
    int ctEXt=0;
    int& ciTXt=ctEXt;
    int czTXt=0;
    int cfRAc=0;
    int cgIFg=0;
    int cgIFt=0;
    int cgIFx=0;
    int cIEND=0;

    using namespace std;

    ifstream fin(file,ios_base::in|ios_base::binary);
    ofstream fout(output,ios_base::out|ios_base::app);

    bool endOfFile=false;
    int count=0;

    auto matchSign=[&](unsigned int* psign)
    {
        if(*psign==*((unsigned int*)IHDR)){++cIHDR;return "IHDR";}
        else if(*psign==*((unsigned int*)cHRM)){++ccHRM;return "cHRM";}
        else if(*psign==*((unsigned int*)gAMA)){++cgAMA;return "gAMA";}
        else if(*psign==*((unsigned int*)iCCP)){++ciCCP;return "iCCP";}
        else if(*psign==*((unsigned int*)sBIT)){++csBIT;return "sBIT";}
        else if(*psign==*((unsigned int*)sRGB)){++csRGB;return "sRGB";}
        else if(*psign==*((unsigned int*)PLTE)){++cPLTE;return "PLTE";}
        else if(*psign==*((unsigned int*)bKGD)){++cbKGD;return "bKGD";}
        else if(*psign==*((unsigned int*)hIST)){++chIST;return "hIST";}
        else if(*psign==*((unsigned int*)tRNS)){++ctRNS;return "tRNS";}
        else if(*psign==*((unsigned int*)oFFs)){++coFFs;return "oFFs";}
        else if(*psign==*((unsigned int*)pHYs)){++cpHYs;return "pHYs";}
        else if(*psign==*((unsigned int*)sPLT)){++csPLT;return "sPLT";}
        else if(*psign==*((unsigned int*)sCAL)){++csCAL;return "sCAL";}
        else if(*psign==*((unsigned int*)IDAT)){++cIDAT;return "IDAT";}
        else if(*psign==*((unsigned int*)tIME)){++ctIME;return "tIME";}
        else if((*psign==*((unsigned int*)tEXt))||(*psign==*((unsigned int*)iTXt))){++ctEXt;return "tEXt";}
        else if(*psign==*((unsigned int*)zTXt)){++czTXt;return "zTXt";}
        else if(*psign==*((unsigned int*)fRAc)){++cfRAc;return "fRAc";}
        else if(*psign==*((unsigned int*)gIFg)){++cgIFg;return "gIFg";}
        else if(*psign==*((unsigned int*)gIFt)){++cgIFt;return "gIFt";}
        else if(*psign==*((unsigned int*)gIFx)){++cgIFx;return "gIFx";}
        else if(*psign==*((unsigned int*)IEND)){++cIEND;endOfFile=true;return "IEND";}
        else{fout<<"ERROR"<<endl;cout<<"ERROR"<<endl;exit(1);}
    };

    unsigned int startPosition=8;
    static const unsigned int SIGNSIZE=4;

    unsigned char lengthBuffer[SIGNSIZE];
    unsigned char signBuffer[SIGNSIZE];
    unsigned char crcCodeBuffer[SIGNSIZE];
    unsigned int *pvalue;
    unsigned int *psign;

    fout<<"-------"<<file<<"-------"<<endl;
    time_t current=time(0);
    tm nowtm;
    tm* pnowtm=&nowtm;
    pnowtm=localtime(&current);
    char aBuffer[100];
    strftime(aBuffer,100,"%Y.%m.%d-%H:%M:%S",pnowtm);
    fout<<"-----"<<aBuffer<<"-----"<<endl;
    fin.seekg(16);
    fin.read((char*)lengthBuffer,SIGNSIZE);
    reverse(lengthBuffer,SIGNSIZE);
    fout<<"宽度---"<<*((unsigned int*)lengthBuffer)<<"px   ";
    fin.read((char*)lengthBuffer,SIGNSIZE);
    reverse(lengthBuffer,SIGNSIZE);
    fout<<"高度---"<<*((unsigned int*)lengthBuffer)<<"px"<<endl;
    fout<<"位置(dec)   位置(hex)   类型\t长度(dec,Byte)\t CRC校验码(hex)\t  数据(hex)"<<endl;
    fin.seekg(startPosition);
    while(!endOfFile&&count<100000000)
    {
        ios_base::fmtflags old=fout.setf(ios_base::left,ios_base::adjustfield);
        fout<<setw(10)<<setfill(' ')<<dec<<fin.tellg();
        fout<<setw(10)<<setfill(' ')<<hex<<fin.tellg();
        fout.setf(old,ios_base::adjustfield);

        fin.read((char*)lengthBuffer,SIGNSIZE);
        reverse(lengthBuffer,SIGNSIZE);
        pvalue=(unsigned int*)lengthBuffer;
        fin.read((char*)signBuffer,SIGNSIZE);
        psign=(unsigned int*)signBuffer;
        unsigned char* p=new unsigned char[*pvalue];
        fin.read((char*)p,*pvalue);
        fin.read((char*)crcCodeBuffer,SIGNSIZE);

        fout<<dec<<matchSign(psign)<<'\t';
        
        old=fout.setf(ios_base::left,ios_base::adjustfield);
        fout<<setw(15)<<setfill(' ')<<*pvalue<<'\t';
        fout.setf(old,ios_base::adjustfield);
        
        printArray(crcCodeBuffer,sizeof(crcCodeBuffer),fout);
        fout<<'\t';
        
        printArray(p,*pvalue,fout);
        fout<<endl;
        ++count;
        delete[]p;
    }
    fout<<"IHDR:"<<dec<<cIHDR<<endl;
    fout<<"PLTE:"<<dec<<cPLTE<<endl;
    fout<<"IDAT:"<<dec<<cIDAT<<endl;
    fout<<"IEND:"<<dec<<cIEND<<endl;
    fout<<"cHRM:"<<dec<<ccHRM<<endl;
    fout<<"gAMA:"<<dec<<cgAMA<<endl;
    fout<<"iCCP:"<<dec<<ciCCP<<endl;
    fout<<"sBIT:"<<dec<<csBIT<<endl;
    fout<<"sRGB:"<<dec<<csRGB<<endl;
    fout<<"bKGD:"<<dec<<cbKGD<<endl;
    fout<<"hIST:"<<chIST<<endl;
    fout<<"tRNS:"<<ctRNS<<endl;
    fout<<"oFFs:"<<coFFs<<endl;
    fout<<"pHYs:"<<cpHYs<<endl;
    fout<<"sPLT:"<<csPLT<<endl;
    fout<<"sCAL:"<<csCAL<<endl;
    fout<<"tIME:"<<ctIME<<endl;
    fout<<"tEXt:"<<ctEXt<<endl;
    fout<<"zTXt:"<<czTXt<<endl;
    fout<<"fRAc:"<<cfRAc<<endl;
    fout<<"gIFg:"<<cgIFg<<endl;
    fout<<"gIFt:"<<cgIFt<<endl;
    fout<<"gIFx:"<<cgIFx<<endl;
    unsigned int abc=fin.tellg();
    if(abc<1024)
        fout<<"图像文件大小---"<<dec<<abc<<"B"<<endl;
    else if(abc<(1024*1024))
        fout<<"图像文件大小---"<<dec<<(float)abc/1024.f<<"KB"<<endl;
    else if(abc<(1024*1024*1024))
        fout<<"图像文件大小---"<<dec<<(float)abc/(1024.f*1024.f)<<"MB"<<endl;
    else
        fout<<"图像文件大小---"<<dec<<(float)abc/(1024.f*1024.f*1024.f)<<"B"<<endl;
}

static void printArray(unsigned char *array, unsigned int len,std::ofstream& fout)
{
    using namespace std;
    for(unsigned int i=0;i<len;i++)
    {
        fout<<setw(2)<<setfill('0')<<hex<<(int)array[i]<<' ';
    }
    setfill(' ');
}
