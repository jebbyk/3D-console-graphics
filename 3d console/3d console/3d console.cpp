// 3d console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "ScreenProc.h"
#include "Geometry.h"
#include <chrono>
#include <math.h>


int main()
{
	SetScreen(180, 320, 4); // minimum recomended values: 24, 32, 2 ; recomnded: 72,128,8

	auto currentTime = std::chrono::steady_clock::now();
	auto previousTime = std::chrono::steady_clock::now();
	float sessionTime = 0;
	float frameTime = 0;
	float averageFrameTimes = 0;
	short framesPerSeccond = 0;

	wchar_t title[64];
	float titleUpdateInterval = 0.3f;
	float titleTimer = 0;
	long framesCount = 0;

	mesh basicCube;// it's just a simple unitCube;
	/*basicCube.triangles = {
	//front
	{0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f},
	//back
	{0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},
	//right
	{1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f},
	//left
	{0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f},
	//top
	{0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f},
	//down
	{0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 1.0f}
	};*/

	basicCube.LoadFromObjFile("SpaceFighter3.obj");

	vector3f lightDirection = { 0.8f, -0.8f, -1.0f };//light//////////////////////////////////////
	float lightL = sqrtf(lightDirection.x * lightDirection.x + lightDirection.y*lightDirection.y + lightDirection.z* lightDirection.z);
	lightDirection.x /= lightL; lightDirection.y /= lightL;  lightDirection.z /= lightL;


	vector3f cameraVector = { 0,0,0 };

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 60.0;
	float aspectRatio = (float)screenHeight / (float)screenWidth;

	float fovRad = 1.0f / tanf(fov*0.5f / 180 * 3.14159f);

	matrix4x4 projectionMatrix;//screen projection matrix;
	projectionMatrix.matrix[0][0] = aspectRatio * fovRad;
	projectionMatrix.matrix[1][1] = fovRad;
	projectionMatrix.matrix[2][2] = farPlane / (farPlane - nearPlane);
	projectionMatrix.matrix[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projectionMatrix.matrix[2][3] = 1.0f;
	projectionMatrix.matrix[3][3] = 0.0f;

	float angle = 0; // just for testing rotating meshes

	while (1)
	{
		currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		frameTime = elapsedTime.count();
		framesCount++;
		sessionTime += frameTime;
		averageFrameTimes = sessionTime / framesCount;

		float offset = sin(sessionTime);// add some movement to benchmark triangles;
		float lightIntancity = 1 + offset;
		float offset2 = cos(sessionTime * 2);
		float offset3 = -sin(sessionTime * 2);

		/*-lightDirection.x = offset;
		lightDirection.y = offset2;
		lightDirection.z = offset3;*/

		matrix4x4 matRotZ, matRotX;

		angle += 1.0f * frameTime;

		matRotZ.matrix[0][0] = cosf(angle);
		matRotZ.matrix[0][1] = sinf(angle);
		matRotZ.matrix[1][0] = -sinf(angle);
		matRotZ.matrix[1][1] = cosf(angle);
		matRotZ.matrix[2][2] = 1;
		matRotZ.matrix[3][3] = 1;

		matRotX.matrix[0][0] = 1;
		matRotX.matrix[1][1] = cosf(angle * 0.5f);
		matRotX.matrix[1][2] = sinf(angle * 0.5f);
		matRotX.matrix[2][1] = -sinf(angle * 0.5f);
		matRotX.matrix[2][2] = cosf(angle * 0.5f);
		matRotX.matrix[3][3] = 1;

		//color testing;


		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{
				DrawPoint(j, i, IntanceTo10Levels((float)i / screenHeight), 0x000f);
			}
		}

		for (triangle t : basicCube.triangles)
		{
			triangle triangleProjection, translatedTriangle, rotatedTriangleZ, rotatedTriangleZX;

			MultMatrixVector(t.vertices[0], rotatedTriangleZ.vertices[0], matRotZ);//rotate mesh over z;
			MultMatrixVector(t.vertices[1], rotatedTriangleZ.vertices[1], matRotZ);
			MultMatrixVector(t.vertices[2], rotatedTriangleZ.vertices[2], matRotZ);

			MultMatrixVector(rotatedTriangleZ.vertices[0], rotatedTriangleZX.vertices[0], matRotX);// rotate mesh over X;
			MultMatrixVector(rotatedTriangleZ.vertices[1], rotatedTriangleZX.vertices[1], matRotX);
			MultMatrixVector(rotatedTriangleZ.vertices[2], rotatedTriangleZX.vertices[2], matRotX);

			translatedTriangle = rotatedTriangleZX;
			translatedTriangle.vertices[0].z = rotatedTriangleZX.vertices[0].z + 6.0f;// translate it from camera position;
			translatedTriangle.vertices[1].z = rotatedTriangleZX.vertices[1].z + 6.0f;
			translatedTriangle.vertices[2].z = rotatedTriangleZX.vertices[2].z + 6.0f;

			vector3f normal, line1, line2;//normal of triangle
			line1.x = translatedTriangle.vertices[1].x - translatedTriangle.vertices[0].x;
			line1.y = translatedTriangle.vertices[1].y - translatedTriangle.vertices[0].y;
			line1.z = translatedTriangle.vertices[1].z - translatedTriangle.vertices[0].z;

			line2.x = translatedTriangle.vertices[2].x - translatedTriangle.vertices[0].x;
			line2.y = translatedTriangle.vertices[2].y - translatedTriangle.vertices[0].y;
			line2.z = translatedTriangle.vertices[2].z - translatedTriangle.vertices[0].z;

			normal.x = line1.y*line2.z - line1.z*line2.y;
			normal.y = line1.z*line2.x - line1.x*line2.z;
			normal.z = line1.x*line2.y - line1.y*line2.x;

			float normalLength = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			normal.x /= normalLength; normal.y /= normalLength; normal.z /= normalLength;

			float dotProduct = normal.x*(translatedTriangle.vertices[0].x - cameraVector.x);
			dotProduct += normal.y * (translatedTriangle.vertices[0].y - cameraVector.y);
			dotProduct += normal.z * (translatedTriangle.vertices[0].z - cameraVector.z);

			if (dotProduct < 0.0f)
			{
				//projection process;
				MultMatrixVector(translatedTriangle.vertices[0], triangleProjection.vertices[0], projectionMatrix);
				MultMatrixVector(translatedTriangle.vertices[1], triangleProjection.vertices[1], projectionMatrix);
				MultMatrixVector(translatedTriangle.vertices[2], triangleProjection.vertices[2], projectionMatrix);

				triangleProjection.vertices[0].x += 1.0f; triangleProjection.vertices[0].y += 1.0f;
				triangleProjection.vertices[1].x += 1.0f; triangleProjection.vertices[1].y += 1.0f;
				triangleProjection.vertices[2].x += 1.0f; triangleProjection.vertices[2].y += 1.0f;

				triangleProjection.vertices[0].x *= 0.5f * (float)screenWidth;
				triangleProjection.vertices[0].y *= 0.5f *(float)screenHeight;
				triangleProjection.vertices[1].x *= 0.5f * (float)screenWidth;
				triangleProjection.vertices[1].y *= 0.5f *(float)screenHeight;
				triangleProjection.vertices[2].x *= 0.5f * (float)screenWidth;
				triangleProjection.vertices[2].y *= 0.5f *(float)screenHeight;

				float ldp = normal.x*lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;
				ldp *= lightIntancity;
				if (ldp < 0) ldp = 0;
				ldp += 0.10;// ambient light;


				FillTriangle(triangleProjection.vertices[0].x, triangleProjection.vertices[0].y,
					triangleProjection.vertices[1].x, triangleProjection.vertices[1].y,
					triangleProjection.vertices[2].x, triangleProjection.vertices[2].y,
					IntanceTo10Levels(ldp), 0x000f);
				/*DrawTriangle(triangleProjection.vertices[0].x, triangleProjection.vertices[0].y,
					triangleProjection.vertices[1].x, triangleProjection.vertices[1].y,
					triangleProjection.vertices[2].x, triangleProjection.vertices[2].y,
					' ', 0x000f);*/
			}
		}

		/*for (int i = 0; i < 29; i++)
		{
			for (int j = 0; j < 53; j++)
			{

				for (int n = 0; n < 20; n++)
				{
					FillTriangle(0 + j * 6, 2 + offset + i * 6, 4 + j * 6, 3 + offset3 + i * 6, 2 + j * 6, 5 + offset2 + i * 6, 0x2588, 0x000f);
					//DrawTriangle(0 + j * 6, 2 + offset + i * 6, 4 + j * 6, 3 + offset3 + i * 6, 2 + j * 6, 5 + offset2 + i * 6, 0x2588, 0x000f);
				}
			}
		}*/

		PushBuffer();//show builded screen;

		titleTimer += frameTime;//update rendering information
		if (titleTimer > titleUpdateInterval)
		{
			titleTimer = 0;
			swprintf_s(title, 64, L"3D console test.   FPS: %4.1f   AVG: %4.2f    TRIS: %d", 1.0f / frameTime, 1.0f / averageFrameTimes, trisCount);
			SetConsoleTitle(title);
		}
		ClearScreen();
		trisCount = 0;
	}

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