#include <digit2char.h>

#include <assert.h>

static const char HexChars[] = "0123456789ABCDEF";

template <typename T> static inline
char _digit2char (T const digit) {
	assert(digit >= 0 && digit <= 0xf);
	assert(digit < sizeof(HexChars)/sizeof(HexChars[0]));

	char const result = HexChars[digit];
	return result;
}

char digit2char (char digit)			{ return _digit2char(digit); }
char digit2char (unsigned char digit)	{ return _digit2char(digit); }
char digit2char (short digit)			{ return _digit2char(digit); }
char digit2char (unsigned short digit)	{ return _digit2char(digit); }
char digit2char (int digit)				{ return _digit2char(digit); }
char digit2char (unsigned digit)		{ return _digit2char(digit); }
char digit2char (long digit)			{ return _digit2char(digit); }
char digit2char (unsigned long digit)	{ return _digit2char(digit); }
