#ifndef _ParabolaOrigin_h_
#define _ParabolaOrigin_h_

#include <windows.h>

namespace Gdiplus
{
	class Graphics;
};

class ParabolaOrigin
{
public:
	ParabolaOrigin();
	void SetPosition(int x, int y);
	void SetWidth(int newWidth);
	void Draw(HDC hdc, Gdiplus::Graphics& graphics);

	int xPos = 40;
	int yPos = 400;
	int width = 10;
	HBRUSH hOrange;
};

#endif
