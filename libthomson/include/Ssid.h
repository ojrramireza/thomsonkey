#ifndef __SSID_H__
#define __SSID_H__

#include <stddef.h>

///////////////////////////////////////////////////////////

namespace thomson {

///////////////////////////////////////////////////////////

static const size_t SSID_LENGTH = 3;

class Ssid {
public:
	typedef unsigned char	SsidElement;
	typedef SsidElement		Elements[SSID_LENGTH];
	typedef Ssid			Self;

private:
	Elements	ssid;

public:
	SsidElement&		operator [] (size_t i);
	SsidElement const&	operator [] (size_t i) const;
	bool				operator < (Self const& other) const;

	char const*			GetSsid (char (& into)[SSID_LENGTH * 2 + 1]) const;

	Ssid (void);
	Ssid (Self const&);

private:
	// Disallow assigning
	void operator = (Self const&);
};

///////////////////////////////////////////////////////////

}	// thomson

///////////////////////////////////////////////////////////

#endif