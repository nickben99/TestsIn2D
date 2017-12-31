#include "stdafx.h"
#include "SphereCollision.h"
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <chrono>

#include "math\CMatrix.h"

using namespace Gdiplus;
using namespace std::chrono;

SphereCollision::SphereCollision()
	: blueBrush(Gdiplus::Color(255, 0, 0, 255))
	, whiteBrush(Gdiplus::Color(255, 255, 255, 255))
	, greenBrush(Gdiplus::Color(255, 0, 255, 0))
	, whitePen(Gdiplus::Color(255, 255, 255, 255))
{
	Go();
}

void SphereCollision::KeyDown(WPARAM wParam)
{
	CMatrix zRot;
	switch (wParam)
	{
	case VK_UP:
		time += 0.1f;
		break;
	case VK_DOWN:
		time -= 0.1f;
		break;
	default: break;
	}	
	UpdateSphere(0, 'W' == wParam, 'S' == wParam, 'A' == wParam, 'D' == wParam, 'Q' == wParam, 'E' == wParam, 'R' == wParam, 'F' == wParam);
	UpdateSphere(1, 'U' == wParam, 'J' == wParam, 'H' == wParam, 'K' == wParam, 'Y' == wParam, 'I' == wParam, 'O' == wParam, 'L' == wParam);
}

void SphereCollision::UpdateSphere(int sphere, bool up, bool down, bool left, bool right, bool rotNeg, bool rotPos, bool incVeloc, bool decVeloc)
{
	if (up) { startPositions[sphere].y -= 2.0f; }
	if (down) { startPositions[sphere].y += 2.0f; }
	if (left) { startPositions[sphere].x -= 2.0f; }
	if (right) { startPositions[sphere].x += 2.0f; }

	CMatrix zRot;
	if (rotNeg)
	{
		zRot.createZRotationMatrix(0.034f);
		velocity[sphere] = zRot.multiplyRotateVector(velocity[sphere]);
	}

	if (rotPos)
	{
		zRot.createZRotationMatrix(-0.034f);
		velocity[sphere] = zRot.multiplyRotateVector(velocity[sphere]);
	}

	if (incVeloc) 
	{ 
		float prevMag = velocity[sphere].getMagnitude();
		if (CVector::SafeNormalize(velocity[sphere]))
		{
			velocity[sphere] *= (prevMag + 5.0f);
		}
		else
		{
			velocity[sphere] = CVector(1.0f, 0.0f, 0.0f);
		}
	}
	if (decVeloc) { velocity[sphere].setMagnitude(std::fmax(0.0f, velocity[sphere].getMagnitude() - 5.0f)); }
}

void SphereCollision::Go()
{
	active = true;
	startTime = high_resolution_clock::now();
}

void SphereCollision::Move()
{
	if (active)
	{
		duration<double, std::milli> timeSinceStart = high_resolution_clock::now() - startTime;
		double secondsSinceStart = timeSinceStart.count() / 1000.0f;

		if (secondsSinceStart >= time) {
			Go();
		}
	}
}

void SphereCollision::DrawCircle(int xCenterPos, int yCenterPos, int radius, Gdiplus::Graphics& graphics, Gdiplus::SolidBrush& brush)
{
	int xDrawPos = xCenterPos - radius;
	int yDrawPos = yCenterPos - radius;
	int width = 2 * radius;
	graphics.FillEllipse(&brush, xDrawPos, yDrawPos, width, width);
	graphics.DrawEllipse(&whitePen, xDrawPos, yDrawPos, width, width);
}

void SphereCollision::DrawSphereCollisionHeading(Font& font, LinearGradientBrush& brush, Gdiplus::Graphics& graphics)
{
	std::wstring heading(L"Sphere Collision Tests (mode: 1)");
	heading += L"\ntime: ";
	heading += std::to_wstring(time);
	graphics.DrawString(heading.c_str(), -1, &font, PointF((Gdiplus::REAL)0, (Gdiplus::REAL)0), &brush);
}

bool AreSpheresColliding(const CVector& sphereOnePos, float sphereOneRadius, const CVector& sphereTwoPos, float sphereTwoRadius)
{
	float minCollisionDistanceSqr = (sphereOneRadius + sphereTwoRadius)*(sphereOneRadius + sphereTwoRadius);
	float sphereDistanceSqr = (sphereOnePos - sphereTwoPos).dotProduct(sphereOnePos - sphereTwoPos);
	return sphereDistanceSqr <= minCollisionDistanceSqr;
}

float clamp(float val, float min, float max)
{
	val = val > min ? val : min;
	return val < max ? val : max;
}

bool WillMovingSphereCollideWithStaticSphere(const CVector& movingStartPos, float movingRadius, const CVector& movingVelocity,
	const CVector& staticPos, float staticRadius, float time, float& hitTime)
{
	CVector diffToStaticPos = staticPos - movingStartPos;
	CVector normalizedVelocity = movingVelocity;
	normalizedVelocity.normalise();
	float maxDistance = (movingVelocity*time).getMagnitude();
	float projection = clamp(normalizedVelocity.dotProduct(diffToStaticPos), 0.0f, maxDistance);
	CVector closestMovingPos = movingStartPos + normalizedVelocity*projection;
	hitTime = maxDistance > 0.0f ? ((projection / maxDistance)*time) : 0.0f;
	return AreSpheresColliding(staticPos, staticRadius, closestMovingPos, movingRadius); // see previous question answer for implementation of this function
}

// see if two spheres will hit within their velocity range
bool WillSpheresCollide(const CVector& onePos, float oneRad, const CVector& oneVeloc, 
	const CVector& twoPos, float twoRad, const CVector& twoVeloc, float time, float& hitTime)
{
	CVector relativeVelocity = oneVeloc - twoVeloc;
	return WillMovingSphereCollideWithStaticSphere(onePos, oneRad, relativeVelocity, twoPos, twoRad, time, hitTime);
}

void SphereCollision::Draw(HDC hdc, Gdiplus::Graphics& graphics)
{
	Font font(&FontFamily(L"Arial"), 12);
	LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Red, LinearGradientModeHorizontal);
	DrawSphereCollisionHeading(font, brush, graphics);

	duration<double, std::milli> timeSinceStart = high_resolution_clock::now() - startTime;
	double secondsSinceStart = timeSinceStart.count() / 1000.0f;

	float hitTime = 0.0f;
	bool collision = WillSpheresCollide(startPositions[0], radius[0], velocity[0], startPositions[1], radius[1], velocity[1], time, hitTime);

	for (int sphere = 0; sphere < NumSpheres; ++sphere)
	{
		DrawCircle((int)startPositions[sphere].x, (int)startPositions[sphere].y, (int)radius[sphere], graphics, blueBrush); // start position
		
		if (active)
		{
			CVector currPos = startPositions[sphere] + velocity[sphere] * float(secondsSinceStart);
			DrawCircle((int)currPos.x, (int)currPos.y, (int)radius[sphere], graphics, greenBrush); // current position
		}

		if (collision)
		{
			CVector collisionPos = startPositions[sphere] + velocity[sphere] * hitTime;
			DrawCircle((int)collisionPos.x, (int)collisionPos.y, (int)radius[sphere], graphics, whiteBrush); // collision position
		}

		CVector finalPos = startPositions[sphere] + velocity[sphere] * time;
		graphics.DrawLine(&whitePen, (int)startPositions[sphere].x, (int)startPositions[sphere].y, (int)finalPos.x, (int)finalPos.y);
	}
}