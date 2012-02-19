#ifndef __INTEGER_SPLITER_H__
#define __INTEGER_SPLITER_H__

///////////////////////////////////////////////////////////

template <typename Int, const size_t BASE>
class IntegerSplitter {
public:
	static Int	Get (size_t what, Int const& from);

private:
	typedef IntegerSplitter<Int, BASE>	Self;

	// No instances
	IntegerSplitter (void);
	IntegerSplitter (Self const&);
	void operator = (Self const&);
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

template <typename Int, const size_t BASE>
Int IntegerSplitter<Int, BASE>::Get (size_t const what, Int const& from) {
	Int result(from);
	
	for (size_t i = 0; i < what && result != 0; ++i)
		result /= BASE;

	return result? result % BASE : Int(0);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

typedef IntegerSplitter<unsigned char, 10>	ucharsplit10;

///////////////////////////////////////////////////////////

#endif
