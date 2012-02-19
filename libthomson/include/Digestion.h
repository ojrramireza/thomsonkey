#ifndef __DIGESTION_H__
#define __DIGESTION_H__

class Digestion {
public:
	typedef unsigned char uint8_t;
	typedef uint8_t Sha1Digest[20];

	static void Sha1 (uint8_t const* data, size_t dataNumberOfBytes, Sha1Digest& digest);
};

#endif
