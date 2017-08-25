#ifndef _ParabolaTarget_h_
#define _ParabolaTarget_h_

#include <windows.h>

namespace Gdiplus
{
	class Graphics;
};

class ParabolaTarget
{
public:
	ParabolaTarget();
	void SetPosition(int x, int y);
	void SetWidth(int newWidth);

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Draw(HDC hdc, Gdiplus::Graphics& graphics);

	int xPos = 0;
	int yPos = 0;
	int width = 10;
	int xVeloc;
	int yVeloc;
	HBRUSH hOrange;
};

#endif
