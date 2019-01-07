#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "Geometry.h"

short KEY_FRONT = 0x57;
short KEY_BACK = 0x53;
short KEY_LEFT = 0x41;
short KEY_RIGHT = 0x44;
short LMB = 0x01;
short RMB = 0x02;
short MMB = 0x04;
short UP_ARROW = 0x26;
short DOWN_ARROW = 0x28;
short LEFT_ARROW = 0x25;
short RIGHT_ARROW = 0x27;

INPUT_RECORD input[512];
DWORD eventsCount = 0;
HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);


POINT mousePos, mouseOldPos, mouseMovement;
//fuck
POINT operator-(POINT& p2)
{
	POINT np;
	return np;
}


struct KeyInfo {
	bool pressed, released, held;
};

KeyInfo keysInfo[256];

/*KeyInfo getKey(short *keyCode)
{
	short a = *keyCode;
	return keysInfo[a];
}*/


void UpdateInputs()
{
	
	mouseOldPos = mousePos;
	GetCursorPos(&mousePos);
	mouseMovement.x = mousePos.x - mouseOldPos.x;
	mouseMovement.y = mousePos.y - mouseOldPos.y;

	short keyState;
	for (int i = 0; i < 256; i++)
	{
		keyState = GetAsyncKeyState(i);
		if (keyState & 0x8000)//first bit shows is key pressed or not
		{
			keysInfo[i].pressed = !keysInfo[i].held;
			keysInfo[i].held = true;
		}
		else {
			keysInfo[i].released = keysInfo[i].held;
			keysInfo[i].held = false;
		}
	}
}
