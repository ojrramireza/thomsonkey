#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Ssid.h>
#include <stddef.h>

///////////////////////////////////////////////////////////

namespace thomson {

///////////////////////////////////////////////////////////

static const size_t SERIAL_LENGTH = 13;

class Serial {
public:
	typedef char			SerialElement;
	typedef SerialElement	Elements[SERIAL_LENGTH];

private:
	Elements	serial;

public:

	void		SetYear (unsigned char year);	// year in [0, 99] for 2000 - 2099
	void		SetWeek (unsigned char week);	// week in [1, 52] for the first to the 52nd week of a year
	void		SetSalt0 (unsigned char salt);	// [0, 36]
	void		SetSalt1 (unsigned char salt);	// [0, 36]
	void		SetSalt2 (unsigned char salt);	// [0, 36]

	struct		Password { char pass[11]; };
	void		GetSsid (Ssid* into, Password* pass) const;

	char const*	GetSerial (void) const;

	Serial (void);

private:
	SerialElement& operator [] (size_t i);
	void SetSalt (size_t, unsigned char);

	typedef Serial Self;
	// Disallow copying
	Serial (Self const&);
	void operator = (Self const&);
};

///////////////////////////////////////////////////////////

}	// thomson

///////////////////////////////////////////////////////////

#endif
