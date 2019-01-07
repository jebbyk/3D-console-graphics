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
#include "Input.h"


int main()
{
	SetScreen(144, 265, 6); // minimum recomended values: 24, 32, 2 ; recomnded: 72,128,8

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

	mesh model;
	model.LoadFromObjFile("groundLow.obj");

	vector3f lightDirection = { 0.8f, -0.8f, -1.0f };//light//////////////////////////////////////
	float lightL = sqrtf(lightDirection.x * lightDirection.x + lightDirection.y*lightDirection.y + lightDirection.z* lightDirection.z);
	lightDirection.x /= lightL; lightDirection.y /= lightL;  lightDirection.z /= lightL;

	vector3f cameraVector = { 0,0,0 };
	vector3f LookDir;
	float yaw = 0, pitch = 0;

	matrix4x4 projectionMatrix;//screen projection matrix;
	projectionMatrix = ScreenProjectionMatrix(60.0, (float)screenHeight / (float)screenWidth, 0.1f, 1000.0f);

	float Xangle = 0; // just for testing rotating meshes
	float Zangle = 0;

	matrix4x4 translationMatrix;
	translationMatrix = MatrixTranslation(0.0f, 0.0f, 8.0f);
	vector3f sOffsetVector = { 1,1,0 };
	

	while (1)
	{
		currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		frameTime = elapsedTime.count();
		framesCount++;
		sessionTime += frameTime;
		averageFrameTimes = sessionTime / framesCount;

		UpdateInputs();

		float lightIntancity = 1;

		matrix4x4 matRotZ, matRotX;
		matRotZ = MatrixRotationZ(Zangle);
		matRotX = MatrixRotationX(Xangle);

		matrix4x4 worldMatrix;
		worldMatrix = MatrixIdentity();
		worldMatrix = MatrixMultMatrix(matRotZ, matRotX);
		worldMatrix = MatrixMultMatrix(worldMatrix, translationMatrix);
		vector3f vUp = { 0,1,0 };
		vector3f targetVector = { 0,0,1 };
		matrix4x4 cameraRotMatrix = MatrixRotationY(yaw);
		LookDir = MatrixMultVector3f(cameraRotMatrix, targetVector);
		targetVector = cameraVector + LookDir;

		matrix4x4 cameraMatrix = MatrixPointAt(cameraVector, targetVector, vUp); 
		matrix4x4 viewMatrix = MatrixQuickInverse(cameraMatrix);

		//color testing;
			/*for (int i = 0; i < screenHeight; i++)
			{
				DrawPoint(j, i, IntanceTo10Levels((float)i / screenHeight), 0x000f);
			}
		}

		for (triangle t : model.triangles)
		{
			triangle triangleProjection, triangleProcessed, triangleViewed;

			triangleProcessed.vertices[0] = MatrixMultVector3f(worldMatrix, t.vertices[0]);
			triangleProcessed.vertices[1] = MatrixMultVector3f(worldMatrix, t.vertices[1]);
			triangleProcessed.vertices[2] = MatrixMultVector3f(worldMatrix, t.vertices[2]);

			triangleViewed.vertices[0] = MatrixMultVector3f(viewMatrix, triangleProcessed.vertices[0]);
			triangleViewed.vertices[1] = MatrixMultVector3f(viewMatrix, triangleProcessed.vertices[1]);
			triangleViewed.vertices[2] = MatrixMultVector3f(viewMatrix, triangleProcessed.vertices[2]);

			vector3f normal, line1, line2;//normal of triangle
			line1 = triangleProcessed.vertices[1] - triangleProcessed.vertices[0];
			line2 = triangleProcessed.vertices[2] - triangleProcessed.vertices[0];
			normal = line1.coross(line2);
			normal = normal.normalized();

			vector3f vCamRay = triangleProcessed.vertices[0] - cameraVector;
			if(normal.dotProd(vCamRay) < 0.0f)
			{
				//projection process;
				triangleProjection.vertices[0] = MatrixMultVector3f(projectionMatrix, triangleViewed.vertices[0]);
				triangleProjection.vertices[1] = MatrixMultVector3f(projectionMatrix, triangleViewed.vertices[1]);
				triangleProjection.vertices[2] = MatrixMultVector3f(projectionMatrix, triangleViewed.vertices[2]);
				//make none orthogonal view
				triangleProjection.vertices[0] /= triangleProjection.vertices[0].w;
				triangleProjection.vertices[1] /= triangleProjection.vertices[1].w;
				triangleProjection.vertices[2] /= triangleProjection.vertices[2].w;
				//move to the centre of the screen
				triangleProjection.vertices[0] += sOffsetVector;
				triangleProjection.vertices[1] += sOffsetVector;
				triangleProjection.vertices[2] += sOffsetVector;

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
}