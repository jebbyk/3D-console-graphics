#pragma once

#include <iostream>
#include <windows.h>
#include <string.h>

short screenWidth, screenHeight;
COORD dwBufferCoord;
SMALL_RECT writeRegion;

HANDLE hStdOut;
HWND hWindow;
CONSOLE_SCREEN_BUFFER_INFO consoleBufInfo;
CHAR_INFO *lpBuffer;
DWORD charectersWriten;


short *c;



long trisCount;


void SetScreen(short h, short w, short cs )
{

	screenWidth = w;
	screenHeight = h;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hWindow = GetConsoleWindow();

	writeRegion = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	COORD bufferSize = { w,h };
	
	SetConsoleScreenBufferSize(hStdOut, bufferSize);//setting up char buffer of console
	
	SetConsoleActiveScreenBuffer(hStdOut);


	//setting up font
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
	//setting up window position and size
	SetWindowPos(hWindow, HWND_TOP, 0, 0, (int)w*cs+16, (int)h*cs+39, NULL);
	writeRegion = {0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	lpBuffer = new CHAR_INFO[screenWidth * screenHeight]; // additional buffer for fasest draving;
	memset(lpBuffer, 0, sizeof(CHAR_INFO)*screenWidth*screenHeight);
	

	

	COORD cursorPos;
	cursorPos.X = 0;
	cursorPos.Y = 0;
	SetConsoleCursorPosition(hStdOut, cursorPos);
}

void DrawPoint(short x, short y, short c, short color)
{
	//put a pixel in a buffer;
	if (x >= 0 && x < (short)screenWidth  && y >= 0 && y < (short)screenHeight)
	{
		lpBuffer[y*(short)screenWidth+ x].Char.UnicodeChar = c;
		lpBuffer[y*screenWidth + x].Attributes = color;
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
				py = y1 + dy * (px - x1) / (float)dx;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x2, y2, c, color);
		}
		else {
			for (px = x2; px < x1; px++)
			{
				py = y1 + dy * (px - x1) / (float)dx;
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
				px = x1 + dx * (py - y1) / (float)dy;
				DrawPoint(short(px), (short)py, c, color);
			}
			DrawPoint(x2, y2, c, color);
		}
		else {
			for (py = y2; py < y1; py++)
			{
				px = x1 + dx * (py - y1) / (float)dy;
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
	trisCount++;
}

void FillTriangle(short x1, short y1, short x2, short y2, short x3, short y3, short c, short color)
{
	if(y1 > y2)
	{
		short t = y1; y1 = y2; y2 = t;
		t = x1; x1 = x2; x2 = t;
	}//sort
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
	
	if (triangleHeight > 0)//if it's not a line;
	{
		float pxA, pxB;
		short segmentHeight = y2 - y1;
		short i;
		if (segmentHeight > 0)//if its not a line again
		{
			float t;
			//draw first segement
			for (short py = y1; py <= y2; py++)
			{
				pxA = (float)(py - y1) / triangleHeight;
				pxB = (float)(py - y1) / segmentHeight;

				pxA = x1 + (x3 - x1)*pxA;
				pxB = x1 + (x2 - x1)*pxB;
				if (pxA > pxB) {
					t = pxA; pxA = pxB; pxB = t;
				}
				for (i = pxA; i <= pxB; i++)
				{
					DrawPoint(i, py, c, color);
				}
			}
		}
		segmentHeight = y3 - y2 ;
		if (segmentHeight > 0)
		{
			float t;
			//draw second segment
			for (short py = y2; py <= y3; py++)
			{
				pxA = (float)(py - y1) / (float)triangleHeight;
				pxB = (float)(py - y2) / (float)segmentHeight;
				pxA = x1 + (float)(x3 - x1)*pxA;
				pxB = x2 + (float)(x3 - x2)*pxB;
				if (pxA > pxB) {
					 t = pxA; pxA = pxB; pxB = t;
				}
				for (i = pxA; i <= pxB; i++)
				{
					DrawPoint(i, py, c, color);
				}
			}
		}
	}

	trisCount++;
}

void ClearScreen()
{
	for (int i = screenWidth * screenHeight; i > 0; i--)
	{
		lpBuffer[i].Char.UnicodeChar = ' ';// just fill buffer full of spaces;
		lpBuffer[i].Attributes = 0x000f;
	}
}

short IntanceTo5Levels(float intancity)
{
	short c;
	if (intancity < 0.2f) c = ' ';
	else {
		if (intancity < 0.4f) c = 0x2591;
		else {
			if (intancity < 0.6f) c = 0x2592;
			else {
				if(intancity < 0.8f)c = 0x2593;
				else {
					c = 0x2588;
				}
			}
		}
	}
	return c;
}

short IntanceTo10Levels(float intance)//It's strongly recomended to use with 8px pixelSize or grate;
{
	short c;

	if (intance < 0.1) c = ' ';
	else  if (intance < 0.2) c = '.'; 
	else if (intance < 0.3) c = ':';
	else if (intance < 0.4) c = '!';
	else if (intance < 0.5) c = 0x2591;
	else if (intance < 0.6) c = 0x2573;
	else if (intance < 0.7) c = '@';
	else if (intance < 0.8) c = 0x2592;
	else if (intance < 0.9) c = 0x2593;
	else  c = 0x2588;
	return c;
}


void PushBuffer()
{
	WriteConsoleOutput(hStdOut, lpBuffer, { screenWidth, screenHeight }, { 0,0 }, &writeRegion);
}
