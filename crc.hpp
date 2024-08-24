#ifndef Crc_hpp_
#define Crc_hpp_

//按字节倒序输入数组
void reverse(unsigned char* ar,unsigned int len);
//按位倒序输入数
unsigned int reverse(unsigned int input);
//使用CRC-32
unsigned int crc32(unsigned char*addr,unsigned int num);

#endif