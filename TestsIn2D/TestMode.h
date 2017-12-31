#ifndef _TestMode_h_
#define _TestMode_h_

#include <windows.h>

namespace Gdiplus
{
	class Graphics;
};

class TestMode
{
public:
	virtual void Move() {};
	virtual void Draw(HDC /* hdc */, Gdiplus::Graphics& /* graphics */) = 0;
	virtual void KeyDown(WPARAM /* wParam */) {};
};

#endif // _TestMode_h_
