#include "stdafx.h"
#include "ParabolaProjectile.h"
#include "ParabolaOrigin.h"
#include "ParabolaTarget.h"
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <chrono>

using namespace Gdiplus;
using namespace std::chrono;

ParabolaProjectile::ParabolaProjectile(ParabolaOrigin* oriIn, ParabolaTarget* targIn)
	: ori(oriIn)
	, targ(targIn)
{
	hOrange = CreateSolidBrush(RGB(255, 180, 0));
	prevTime = high_resolution_clock::now();
}

void ParabolaProjectile::SetWidth(int newWidth)
{
	width = newWidth;
}

void ParabolaProjectile::Go()
{
	active = true;
	startTime = prevTime;
}

void ParabolaProjectile::Move()
{
	high_resolution_clock::time_point currTime = high_resolution_clock::now();
	duration<double, std::milli> time_span = currTime - prevTime;
	prevTime = currTime;
	double frameDeltaMilliseconds = time_span.count();
	double frameDeltaSeconds = frameDeltaMilliseconds / 1000.0f;

	if (active)
	{
		duration<double, std::milli> timeSinceStart = currTime - startTime;
		double secondsSinceStart = timeSinceStart.count() / 1000.0f;

		velocX = (targ->xPos - ori->xPos) / flightDuration;
		velocY = (targ->yPos + arc * -9.8f * flightDuration * flightDuration - ori->yPos) / flightDuration;

		xPos = int(velocX*secondsSinceStart + ori->xPos);
		yPos = int(-arc * -9.8f * secondsSinceStart * secondsSinceStart + velocY * secondsSinceStart + ori->yPos);

		if (secondsSinceStart >= flightDuration) {
			Go();
		}
	}
}

void ParabolaProjectile::Draw(HDC hdc, Gdiplus::Graphics& graphics)
{
	RECT secondbackground;

	int widthDiv2 = (width / 2);
	SetRect(&secondbackground, xPos - widthDiv2, yPos - widthDiv2, xPos + widthDiv2, yPos + widthDiv2);
	FillRect(hdc, &secondbackground, hOrange);

	Font font(&FontFamily(L"Arial"), 12);
	LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Red, LinearGradientModeHorizontal);

	std::wstring posString(L"x: ");
	posString += std::to_wstring(xPos);
	posString += L" y: ";
	posString += std::to_wstring(yPos);
	posString += L" duration: ";
	posString += std::to_wstring(flightDuration);
	posString += L" arc: ";
	posString += std::to_wstring(arc);

	graphics.DrawString(posString.c_str(), -1, &font, PointF((Gdiplus::REAL)xPos, (Gdiplus::REAL)yPos), &brush);
}