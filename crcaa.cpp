#include "crcaa.hpp"
uint8_t crcTable[0x100];
static void makeCrcTable();
static uint32_t updateCrc(uint32_t crc,uint8_t* buffer,const uint32_t& len);
uint32_t crc(uint8_t *data, uint32_t len)
{
    return updateCrc(0xFFFFFFFF,data,len)^0xFFFFFFFF;
}

static void makeCrcTable()
{
    uint32_t c;
    for(uint32_t n=0;n<0x100;n++)
    {
        c=n;
        for(uint32_t k=0;k<8;k++)
            c=(c&1)?(0x4c11db7^(c>>1)):(c>>1);
        crcTable[n]=c;
    }
}

static uint32_t updateCrc(uint32_t crc, uint8_t *buffer,const uint32_t& len)
{
    makeCrcTable();
    for(uint32_t n=0;n<len;n++)
        crc=crcTable[(crc^buffer[n])&0xff]^(crc>>8);
    return crc;
}
