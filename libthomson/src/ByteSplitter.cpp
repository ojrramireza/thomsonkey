#include <ByteSplitter.h>
//
#include <digit2char.h>
//
#include <assert.h>

///////////////////////////////////////////////////////////

static inline bool IsHalfByte (ByteSplitter::Byte const byte) {
	return byte <= 0xf;
}

///////////////////////////////////////////////////////////

ByteSplitter::Byte ByteSplitter::GetLeastSignificantHalfByte (Byte const byte) {
	Byte const result = byte & 0x0f;

	assert(IsHalfByte(result));
	return result;
}

///////////////////////////////////////////////////////////

ByteSplitter::Byte ByteSplitter::GetMostSignificantHalfByte (Byte const byte) {
	Byte const result = (byte & 0xf0) >> 4;

	assert(IsHalfByte(result));
	return result;
}

///////////////////////////////////////////////////////////

void ByteSplitter::ToString (char* const dest, size_t const destLength, Byte const byte) {
	assert(destLength > 2);

	dest[0] = digit2char(GetMostSignificantHalfByte(byte));
	dest[1] = digit2char(GetLeastSignificantHalfByte(byte));
	dest[2] = '\0';
}

///////////////////////////////////////////////////////////
