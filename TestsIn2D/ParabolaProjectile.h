#ifndef _ParabolaProjectile_h_
#define _ParabolaProjectile_h_

#include <windows.h>
#include <chrono>

namespace Gdiplus
{
	class Graphics;
};

class ParabolaOrigin;
class ParabolaTarget;

class ParabolaProjectile
{
public:
	ParabolaProjectile(ParabolaOrigin* ori, ParabolaTarget* targ);
	void SetWidth(int newWidth);

	void Go();
	void Move();
	void Draw(HDC hdc, Gdiplus::Graphics& graphics);

	int xPos = 0;
	int yPos = 0;
	int width = 10;
	HBRUSH hOrange;

	ParabolaOrigin* ori;
	ParabolaTarget* targ;

	std::chrono::high_resolution_clock::time_point prevTime;
	std::chrono::high_resolution_clock::time_point startTime;
	float velocX = 0.0f;
	float velocY = 0.0f;
	bool active = false;
	float flightDuration = 5.0f; // seconds
	float arc = 0.9f;
};

#endif
