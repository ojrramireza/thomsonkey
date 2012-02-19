
#include <assert.h>

///////////////////////////////////////////////////////////

namespace thomson {

///////////////////////////////////////////////////////////

template <typename T>
class Range {
	T const from, to;
	T current;
	bool firstNext;

public:
	typedef Range<T>	Self;

	T const&	GetCurrent (void) const;
	bool		HasNext (void) const;
	T const&	Next (void);
	void		Reset (void);
	
	Range (T const& from, T const& to);
	Range (Self const& other);

private:
	// Disallow assigning
	void operator = (Self const&);
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

template <typename T> inline
T const& Range<T>::GetCurrent (void) const {
	assert(!firstNext);
	return current;
}

///////////////////////////////////////////////////////////

template <typename T> inline
void Range<T>::Reset (void) {
	current = from;
	firstNext = true;
}

///////////////////////////////////////////////////////////

template <typename T> inline
bool Range<T>::HasNext (void) const {
	return firstNext || !(current == to);
}

///////////////////////////////////////////////////////////

template <typename T> inline
T const& Range<T>::Next (void) {
	assert(HasNext());

	if (firstNext)
		firstNext = false;
	else
		++current;

	return current;
}

///////////////////////////////////////////////////////////

template <typename T> inline
Range<T>::Range (T const& from, T const& to):
	from		(from	),
	to			(to		),
	current		(from	),
	firstNext	(true	) {
	}

template <typename T> inline
Range<T>::Range (Self const& other):
	from		(other.from			),
	to			(other.to			),
	current		(other.current		),
	firstNext	(other.firstNext	) {
	}

///////////////////////////////////////////////////////////

}	// thomson

///////////////////////////////////////////////////////////
