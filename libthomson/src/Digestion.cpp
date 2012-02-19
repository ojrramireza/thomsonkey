#include <Digestion.h>
//
#include <sha1.h>
//
#include <assert.h>

void Digestion::Sha1 (uint8_t const* const data, size_t const len, Sha1Digest& digest) {
	SHA1Context sha1;
	assert(sizeof(Sha1Digest) == sizeof(uint8_t [20]));

	SHA1Reset(&sha1);
	SHA1Input(&sha1, data, len);
	SHA1Result(&sha1, &digest[0]);
}
