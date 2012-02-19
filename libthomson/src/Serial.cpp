#include <Serial.h>

#include <safeconvert.h>
#include <IntegerSplitter.h>
#include <digit2char.h>
#include <ByteSplitter.h>
#include <Digestion.h>
//
#include <string.h>
#include <assert.h>

///////////////////////////////////////////////////////////

static const char CharTable[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static inline char tochar (size_t const i) {
	assert(i < sizeof(CharTable)/sizeof(CharTable[0]));

	return CharTable[i];
}

///////////////////////////////////////////////////////////

namespace thomson {

///////////////////////////////////////////////////////////

void Serial::SetYear (unsigned char const year) {
	assert(year < 100);

	unsigned char const decade = ucharsplit10::Get(1, year);
	unsigned char const monade = ucharsplit10::Get(0, year);

	char const decadeChar = digit2char(decade);
	char const monadeChar = digit2char(monade);

	safeconvert((*this)[2], decadeChar);
	safeconvert((*this)[3], monadeChar);
}

///////////////////////////////////////////////////////////

void Serial::SetWeek (unsigned char const week) {
	assert(week >= 1 && week <= 52);

	unsigned char const decade = ucharsplit10::Get(1, week);
	unsigned char const monade = ucharsplit10::Get(0, week);

	char const decadeChar = digit2char(decade);
	char const monadeChar = digit2char(monade);

	safeconvert((*this)[4], decadeChar);
	safeconvert((*this)[5], monadeChar);
}

///////////////////////////////////////////////////////////

void Serial::SetSalt0 (unsigned char const salt) {
	SetSalt(6, salt);
}

///////////////////////////////////////////////////////////

void Serial::SetSalt1 (unsigned char const salt) {
	SetSalt(8, salt);
}

///////////////////////////////////////////////////////////

void Serial::SetSalt2 (unsigned char const salt) {
	SetSalt(10, salt);
}

///////////////////////////////////////////////////////////

void Serial::GetSsid (Ssid* const into, Password* const password) const {
	Digestion::Sha1Digest digest;

	Digestion::Sha1(
			reinterpret_cast<Digestion::uint8_t const*>(&serial[0]),
			SERIAL_LENGTH - 1,	// do not hash null string terminator too
			digest);

	if (into) {
		safeconvert((*into)[0], digest[17]);
		safeconvert((*into)[1], digest[18]);
		safeconvert((*into)[2], digest[19]);
	}

	if (password) {
		ByteSplitter::ToString(&password->pass[0], 3, digest[0]);
		ByteSplitter::ToString(&password->pass[2], 3, digest[1]);
		ByteSplitter::ToString(&password->pass[4], 3, digest[2]);
		ByteSplitter::ToString(&password->pass[6], 3, digest[3]);
		ByteSplitter::ToString(&password->pass[8], 3, digest[4]);
		assert(password->pass[10] == '\0');
	}
}

///////////////////////////////////////////////////////////

void Serial::SetSalt (size_t const startingIndex, unsigned char const salt) {
	assert(salt <= 36);

	char const saltChar = tochar(salt);
	ByteSplitter::Byte const saltCharByte(safeconvert<ByteSplitter::Byte>(saltChar));

	assert(startingIndex + 3 <= SERIAL_LENGTH);
	ByteSplitter::ToString(&serial[startingIndex], 3, saltCharByte);
}

///////////////////////////////////////////////////////////

Serial::Serial (void):
	serial	(	) {

	memset(&serial, 0x00, sizeof(serial));

	safeconvert(serial[0], 'C');
	safeconvert(serial[1], 'P');
	safeconvert(serial[2], '0');
}

///////////////////////////////////////////////////////////

Serial::SerialElement& Serial::operator [] (size_t const i) {
	assert(i >= 0u && i <= SERIAL_LENGTH);

	return serial[i];
}

///////////////////////////////////////////////////////////

}	// thomson

///////////////////////////////////////////////////////////
