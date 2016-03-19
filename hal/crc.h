#pragma once
#ifndef CRC_H
#define CRC_H

#include "stdint.h"

namespace Mcudrv {
namespace Crc {

//Maxim-Dallas computation (X8 + X5 + X4 + 1)
class Crc8
{
private:
	typedef Crc8 Self;
	static const uint8_t table[256];
	uint8_t crc_;
public:
	Crc8(uint8_t init = 0) : crc_(init)
	{	}
	Self& Reset(uint8_t init = 0)
	{
		crc_ = init;
		return *this;
	}
	Self& Eval(uint8_t value)
	{
		operator()(value);
		return *this;
	}
	void operator()(uint8_t value)
	{
		crc_ = table[crc_ ^ value];
	}
	operator()(const uint8_t* buf, uint8_t len, uint8_t initValue = 0)
	{
		crc_ = initValue;
		for(uint8_t i = 0; i < len; ++i)
		{
			crc_ = table[crc_ ^ buf[i]];
		}
		return crc_;
	}
	uint8_t Get()
	{
		return crc_;
	}
};

//direct compute #1
/*	uint8_t CrcN1(const uint8_t *addr, uint8_t len) {
	  uint8_t crc = 0;
	  while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
		  uint8_t mix = (crc ^ inbyte) & 0x01;
		  crc >>= 1;
		  if (mix) crc ^= 0x8C;
		  inbyte >>= 1;
		}
	  }
	  return crc;
	}
//Direct compute #2
uint8_t CrcN2(const uint8_t* buf, uint32_t len)
{
	uint8_t crc = 0;
	while(len--)
	{
		uint8_t b = *buf++;
		for(char i = 0; i < 8; b = b >> 1, i++)
			if((b ^ crc) & 1) crc = ((crc ^ 0x18) >> 1) | 0x80;
			else crc = (crc >> 1) & ~0x80;
	}
	return crc;
}
*/

}//Crc
}//Mcudrv

#endif // CRC_H

