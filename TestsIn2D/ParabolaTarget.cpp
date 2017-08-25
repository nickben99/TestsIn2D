#include "stdafx.h"
#include "ParabolaTarget.h"
#include <objidl.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;

ParabolaTarget::ParabolaTarget()
	: xVeloc(10)
	, yVeloc(10)
{
	hOrange = CreateSolidBrush(RGB(255,180,0));
}

void ParabolaTarget::SetPosition(int x, int y)
{
	xPos = x;
	yPos = y;
}

void ParabolaTarget::SetWidth(int newWidth)
{
	width = newWidth;
}

void ParabolaTarget::MoveLeft()
{
	SetPosition(xPos-xVeloc, yPos);
}

void ParabolaTarget::MoveRight()
{
	SetPosition(xPos + xVeloc, yPos);
}

void ParabolaTarget::MoveUp()
{
	SetPosition(xPos, yPos - yVeloc);
}

void ParabolaTarget::MoveDown()
{
	SetPosition(xPos, yPos + yVeloc);
}

void ParabolaTarget::Draw(HDC hdc, Gdiplus::Graphics& graphics)
{
	RECT secondbackground;

	int widthDiv2 = (width/2);
	SetRect(&secondbackground, xPos-widthDiv2, yPos-widthDiv2, xPos+widthDiv2, yPos+widthDiv2);
    FillRect(hdc, &secondbackground, hOrange);

	Font font(&FontFamily(L"Arial"), 12);
	LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Red, LinearGradientModeHorizontal);

	std::wstring posString(L"x: ");
	posString += std::to_wstring(xPos);
	posString += L" y: ";
	posString += std::to_wstring(yPos);

	graphics.DrawString(posString.c_str(), -1, &font, PointF((Gdiplus::REAL)xPos, (Gdiplus::REAL)yPos), &brush);
}