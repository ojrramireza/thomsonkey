#include <Ssid.h>
//
#include <ByteSplitter.h>
//
#include <assert.h>
#include <string.h>

#define GET_ELEMENT(INDEX)	ssid[toindex((INDEX))]

static inline size_t toindex (size_t const value) {
	assert(value >= 0u && value <= thomson::SSID_LENGTH);
	
	return value;
}

///////////////////////////////////////////////////////////

namespace thomson {

///////////////////////////////////////////////////////////

Ssid::SsidElement& Ssid::operator [] (size_t const i) {
	return GET_ELEMENT(i);
}

Ssid::SsidElement const& Ssid::operator [] (size_t const i) const {
	return GET_ELEMENT(i);
}

///////////////////////////////////////////////////////////

bool Ssid::operator < (Self const& other) const {
	return memcmp(&ssid[0], &other.ssid[0], sizeof(ssid)) < 0;
}

///////////////////////////////////////////////////////////

char const* Ssid::GetSsid (char (& into)[SSID_LENGTH * 2 + 1]) const {
	assert(SSID_LENGTH == 3);

	ByteSplitter::ToString(&into[0], 3, ssid[0]);
	ByteSplitter::ToString(&into[2], 3, ssid[1]);
	ByteSplitter::ToString(&into[4], 3, ssid[2]);

	return &into[0];
}

///////////////////////////////////////////////////////////

Ssid::Ssid (void):
	ssid	(	) {
}

Ssid::Ssid (Self const& other):
	ssid	(	) {
	memcpy(&ssid[0], &other.ssid[0], sizeof(ssid));
}

///////////////////////////////////////////////////////////

}	// thomson

///////////////////////////////////////////////////////////
