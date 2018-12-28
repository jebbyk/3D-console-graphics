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

void ClearScreen()
{
	system("CLS");
}

COORD screenSize;
COORD dwBufferCoord;
PSMALL_RECT writeRegion;

HANDLE hStdOut;
HWND hWindow;
CONSOLE_SCREEN_BUFFER_INFO consoleBufInfo;
CHAR_INFO *lpBuffer = new CHAR_INFO[screenSize.X * screenSize.Y]; // additional buffer
void PushBuffer()
{
	WriteConsoleOutput(hStdOut, lpBuffer, screenSize, screenSize, writeRegion);
}


void SetScreen(short h, short w, short ch, short cw)
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hWindow = GetConsoleWindow();
	GetConsoleScreenBufferInfo(hStdOut, &consoleBufInfo);
	
	SMALL_RECT windowSize = { 0,0, w - 1, h - 1 };
	SetConsoleWindowInfo(hStdOut, false, &windowSize);

	COORD bufferSize = { h,w };
	SetConsoleScreenBufferSize(hStdOut, bufferSize);


	COORD cursorPos;
	cursorPos.X = 0;
	cursorPos.Y = 0;
	SetConsoleCursorPosition(hStdOut, cursorPos);
}
