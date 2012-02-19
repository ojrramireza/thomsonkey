#include <Windows.h>
#include <tchar.h>
#include <assert.h>

// return: number of chars copied (narrowed) (excluding EOS)
static size_t narrowen (char* const dest, size_t const destLength, wchar_t const* const src, size_t const n) {
	size_t i;
	for (i = 0; i < n; ++i) {
		if (src[i] & 0xff00)
			throw "true wide string -- cannot simple-convert";
		assert(i < destLength);
		dest[i] = static_cast<char>(src[i]);
	}

	assert(i <= destLength);
	if (i == destLength)
		--i;
	dest[i] = '\0';

	return i + 1;
}


int APIENTRY _tWinMain (
		HINSTANCE hInstance			,
		HINSTANCE hPrevInstance		,
		TCHAR* lpCmdLine			,
		int nCmdShow
) {

	
	return 0;
}

