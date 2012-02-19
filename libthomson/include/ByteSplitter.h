#ifndef __BYTE_SPLITTER_H__
#define __BYTE_SPLITTER_H__

///////////////////////////////////////////////////////////

class ByteSplitter {
public:
	typedef unsigned char	Byte;

	static Byte		GetLeastSignificantHalfByte (Byte);
	static Byte		GetMostSignificantHalfByte (Byte);

	static void		ToString (char* dest, size_t destLength, Byte byte);
private:
	typedef ByteSplitter	Self;

	// No instances or copying
	ByteSplitter (void);
	ByteSplitter (Self const&);
	void operator = (Self const&);
};

///////////////////////////////////////////////////////////

#endif
