﻿// 3d console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "ScreenProc.h"

int main()
{
	SetScreen(72, 128, 8); // minimum recomended values: 45, 71, 4 ; recomnded: 72,128,8
	//_getch();
	//ClearScreen();
	//_getch();
	//DrawLine(50, 50, 0, 0, 0x2588, 0x000f);
	DrawTriangle(10, 10, 48, 14, 24, 64, 0x2588, 0x000f);
	//DrawPoint(10, 10, 0x2588, 0x000f);
	PushBuffer();
	_getch();

	/*while (1)
	{
		DrawTriangle(10, 10, 12, 64, 48, 32);
	}*/

	
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
