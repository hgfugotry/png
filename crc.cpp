#include<iostream>
#include<fstream>
#include<stdint.h>

#define POLY 0x04c11db7
#define INIT 0xffffffff
#define XOROUT 0xffffffff

//按字节倒序输入数组
void reverse(unsigned char *ar,unsigned len)
{
    unsigned char temp;
    for(int i=0,j=len-1;i<len/2;i++,j--)
    {
        temp=ar[i];
        ar[i]=ar[j];
        ar[j]=temp;
    }
}

// 按位倒序输入数
unsigned int reverse(unsigned int input)
{
    unsigned int output=0;
    for(unsigned int i=1;i!=0;i<<=1)
    {
        output<<=1;
        if(input&1)
            output|=1;
        input>>=1;
    }
    return output;
}

//使用CRC-32
unsigned int crc32(unsigned char*addr,unsigned int num)
{
    unsigned int crc=INIT;
    while(num-->0)
    {
        crc^=reverse(*addr++);
        for(int i=0;i<8;i++)
            crc=((crc&0x80000000)?((crc<<1)^POLY):(crc<<1));
        crc&=0xffffffff;
    }
    return reverse(crc^XOROUT);
}