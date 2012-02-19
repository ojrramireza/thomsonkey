#ifndef __SAFE_CONVERT_H__
#define __SAFE_CONVERT_H__

///////////////////////////////////////////////////////////

struct bad_conversion {};

///////////////////////////////////////////////////////////

template <typename To, typename From> extern
void safeconvert (To&, From const&);

///////////////////////////////////////////////////////////

template <typename T> inline static
void safeconvert (T& to, T const& from) {
	to = from;
}

///////////////////////////////////////////////////////////

template <typename To, typename From> static inline
To safeconvert (From const& from) {
	To to;
	safeconvert(to, from);
	return to;
}

///////////////////////////////////////////////////////////

#endif
