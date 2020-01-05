#pragma once
#include <stdint.h>

class Bitmask
{
public:
	Bitmask(){}
	Bitmask(uint32_t bits) : bits(bits){}
	
	void setMask(Bitmask& other);
	uint32_t getMask() const;
	bool getBit(int pos) const;
	void setBit(int pos, bool on);
	void setBit(int pos);
	void clearBit(int pos);
	void clear();
private:
	uint32_t bits;
};

