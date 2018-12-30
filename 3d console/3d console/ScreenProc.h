#pragma once

#include <iostream>
#include <windows.h>
#include <string.h>

COORD screenSize;
COORD dwBufferCoord;
SMALL_RECT writeRegion;

HANDLE hStdOut;
HWND hWindow;
CONSOLE_SCREEN_BUFFER_INFO consoleBufInfo;
CHAR_INFO *lpBuffer;
DWORD charectersWriten;


void SetScreen(short h, short w, short cs )
{

	screenSize.X = w;
	screenSize.Y = h;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hWindow = GetConsoleWindow();

	writeRegion = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	COORD bufferSize = { w,h };
	
	SetConsoleScreenBufferSize(hStdOut, bufferSize);
	
	SetConsoleActiveScreenBuffer(hStdOut);



	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = cs;
	fontInfo.dwFontSize.Y = cs;
	fontInfo.FontFamily = FF_DECORATIVE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hStdOut, false, &fontInfo);
	SetConsoleMode(hStdOut, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	SetWindowPos(hWindow, HWND_TOP, 0, 0, (int)w*cs+16, (int)h*cs+39, NULL);
	writeRegion = {0, 0, (short)screenSize.X - 1, (short)screenSize.Y - 1 };
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	lpBuffer = new CHAR_INFO[screenSize.X * screenSize.Y]; // additional buffer
	memset(lpBuffer, 0, sizeof(CHAR_INFO)*screenSize.X*screenSize.Y);
	

	/*short pos = 0;
	short step = h / 10;
	FillConsoleOutputCharacter(hStdOut, 0x2588, 128 * step, { 0,pos }, &charectersWriten);//solid
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x2593, 128 * step, { 0,pos }, &charectersWriten);//3qsolid
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x2592, 128 * step, { 0,pos }, &charectersWriten);//2qsolid
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x0040, 128 * step, { 0,pos }, &charectersWriten);// @
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x2573, 128 * step, { 0,pos }, &charectersWriten);// AE
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x2591, 128 * step, { 0,pos }, &charectersWriten); // 1qsolid
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x0021, 128 * step, { 0,pos }, &charectersWriten);// !
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x003a, 128 * step, { 0,pos }, &charectersWriten);// :
	pos += step;
	FillConsoleOutputCharacter(hStdOut, 0x02d1, 128 * step, { 0,pos }, &charectersWriten);// &&&&
	pos += step;*/
	

	COORD cursorPos;
	cursorPos.X = 0;
	cursorPos.Y = 0;
	SetConsoleCursorPosition(hStdOut, cursorPos);
}

void DrawPoint(short x, short y, short c, short color)
{
	if (x >= 0 && x < screenSize.X  && y >= 0 && y < screenSize.Y)
	{
		lpBuffer[y*screenSize.X + x].Char.UnicodeChar = c;
		lpBuffer[y*screenSize.X + x].Attributes = color;
	}
}

void DrawLine(short x1, short y1, short x2, short y2, short c, short color)
{
	int px = x1, py = y1, dx = x2 - x1, dy = y2 - y1, adx = abs(dx), ady = abs(dy);

	if (adx >= ady)
	{
		if (dx > 0)
		{
			for (px = x1; px < x2; px++)
			{
				py = y1 + dy * (px - x1) / dx;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x2, y2, c, color);
		}
		else {
			for (px = x2; px < x1; px++)
			{
				py = y1 + dy * (px - x1) / dx;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x1, y1, c, color);
		}
	}
	else {
		if (dy > 0)
		{
			for (py = y1; py < y2; py++)
			{
				px = x1 + dx * (py - y1) / dy;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x2, y2, c, color);
		}
		else {
			for (py = y2; py < y1; py++)
			{
				px = x1 + dx * (py - y1) / dy;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x1, y1, c, color);
		}
	}
}

void DrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, short c, short color)
{
	DrawLine(x1, y1, x2, y2, c, color);
	DrawLine(x2, y2, x3, y3, c, color);
	DrawLine(x3, y3, x1, y1, c, color);
}

void FillTriangle(short x1, short y1, short x2, short y2, short x3, short y3, short c, short color)
{
	if(y1 > y2)
	{
		short t = y1; y1 = y2; y2 = t;
		t = x1; x1 = x2; x2 = t;
	}
	if (y1 > y3)
	{
		short t = y1; y1 = y3; y3 = t;
		t = x1; x1 = x3; x3 = t;
	}
	if (y2 > y3)
	{
		short t = y2; y2 = y3; y3 = t;
		t = x2; x2 = x3; x3 = t;
	}

	short triangleHeight = y3 - y1;
	if (triangleHeight > 0)
	{
		short segmentHeight = y2 - y1;
		if (segmentHeight > 0)
		{
			float pxA, pxB;
			for (short py = y1; py <= y2; py++)
			{
				pxA = (float)(py - y1) / triangleHeight;
				pxB = (float)(py - y1) / segmentHeight;

				pxA = x1 + (x3 - x1)*pxA;
				pxB = x1 + (x2 - x1)*pxB;
				DrawLine(pxA, py, pxB, py, c, color);
				/*if (pxA > pxB) std::swap(pxA, pxB);
				for (short i = pxA; i <= pxB; i++)
				{
					DrawPoint(i, py, c, color);
				}*/
			}
		}
		segmentHeight = y3 - y2 + 1;
		if (segmentHeight > 0)
		{
			float pxA, pxB;
			for (short py = y2; py <= y3; py++)
			{
				pxA = (float)(py - y1) / triangleHeight;
				pxB = (float)(py - y2) / segmentHeight;
				pxA = x1 + (x3 - x1)*pxA;
				pxB = x2 + (x3 - x2)*pxB;
				DrawLine(pxA, py, pxB, py, c, color);
			}
		}
	}
}

void ClearScreen()
{
	FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', screenSize.X*screenSize.Y, { 0,0 }, &charectersWriten);//solid
}

void PushBuffer()
{
	WriteConsoleOutput(hStdOut, lpBuffer, { screenSize.X, screenSize.Y }, { 0,0 }, &writeRegion);
}
