#ifndef LPng_hpp_
#define LPng_hpp_
#include<stdint.h>
class LPng_IHDR
{
public:
    uint8_t length[4];
    uint8_t chunkTyprCode[4]={73,72,68,82};
public:
    uint8_t width[4];
    uint8_t height[4];
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
public:
    uint32_t crc;
};
class LPng
{
public:
    uint8_t name[8]={0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
    LPng_IHDR IHDR;
};
//报告数据块类型
void reportPngDataBlock(const char* file,const char* output);
#endif