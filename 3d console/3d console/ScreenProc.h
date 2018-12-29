#pragma once

#include <iostream>
#include <windows.h>
#include <string.h>

void DrawLine(short x, short y)
{

}

void DrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3)
{

}

void FillTrinangle(short x1, short y1, short x2, short y2, short x3, short y3, char c)
{

}


COORD screenSize;
COORD dwBufferCoord;
PSMALL_RECT writeRegion;

HANDLE hStdOut;
HWND hWindow;
CONSOLE_SCREEN_BUFFER_INFO consoleBufInfo;
CHAR_INFO *lpBuffer;
DWORD charectersWriten;
void PushBuffer()
{
	WriteConsoleOutput(hStdOut, lpBuffer, screenSize, screenSize, writeRegion);
}

void SetScreen(short h, short w, short cs )
{

	screenSize.X = w;
	screenSize.Y = h;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hWindow = GetConsoleWindow();

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

	lpBuffer = new CHAR_INFO[screenSize.X * screenSize.Y]; // additional buffer
	memset(lpBuffer, 0, sizeof(CHAR_INFO)*screenSize.X*screenSize.Y);
	

	short pos = 0;
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
	pos += step;
	

	COORD cursorPos;
	cursorPos.X = 0;
	cursorPos.Y = 0;
	SetConsoleCursorPosition(hStdOut, cursorPos);
}


void ClearScreen()
{
	FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', screenSize.X*screenSize.Y, { 0,0 }, &charectersWriten);//solid
}
