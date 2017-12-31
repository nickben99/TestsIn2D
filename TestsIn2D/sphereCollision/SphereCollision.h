#ifndef _SphereCollision_h_
#define _SphereCollision_h_

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <chrono>

#include "TestMode.h"
#include "math/CVector.h"

namespace Gdiplus
{
	class Graphics;
	class Font;
	class LinearGradientBrush;
};

class SphereCollision : public TestMode
{
public:
	SphereCollision();

	void Go();
	virtual void Move() override;
	virtual void Draw(HDC hdc, Gdiplus::Graphics& graphics) override;
	virtual void KeyDown(WPARAM wParam) override;

private:
	void UpdateSphere(int sphere, bool up, bool down, bool left, bool right, bool rotNeg, bool rotPos, bool increaseVeloc, bool decreaseVeloc);
	void DrawSphereCollisionHeading(Gdiplus::Font& font, Gdiplus::LinearGradientBrush& brush, Gdiplus::Graphics& graphics);
	void DrawCircle(int xCenterPos, int yCenterPos, int radius, Gdiplus::Graphics& graphics, Gdiplus::SolidBrush& brush);

private:
	Gdiplus::SolidBrush blueBrush;
	Gdiplus::SolidBrush whiteBrush;
	Gdiplus::SolidBrush greenBrush;
	Gdiplus::Pen whitePen;

	std::chrono::high_resolution_clock::time_point startTime;
	bool active = false;
	float time = 1.0f;
	static const int NumSpheres = 2;
	CVector startPositions[NumSpheres]{ CVector(100.0f, 200.0f, 0.0f), CVector(400.0f, 200.0f, 0.0f) };
	float radius[NumSpheres]{ 40.0f, 20.0f };
	CVector velocity[NumSpheres]{ CVector(100.0f, 0.0f, 0.0f), CVector(-100.0f, 0.0f, 0.0f) }; // units/second
};

#endif // _SphereCollision_h_
