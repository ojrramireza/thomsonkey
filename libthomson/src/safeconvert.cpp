#include <safeconvert.h>

#include <limits.h>

///////////////////////////////////////////////////////////

template <>
void safeconvert<unsigned char, char> (unsigned char& to, char const& from) {
	if (from < 0)
		throw bad_conversion();

	to = static_cast<unsigned char>(from);
}

///////////////////////////////////////////////////////////

template <>
void safeconvert<char, unsigned char> (char& to, unsigned char const& from) {
	if (from > CHAR_MAX)
		throw bad_conversion();

	to = static_cast<char>(from);
}

///////////////////////////////////////////////////////////

template <>
void safeconvert<char, int> (char& to, int const& from) {
	if (from < CHAR_MIN || from > CHAR_MAX)
		throw bad_conversion();

	to = static_cast<char>(from);
}
