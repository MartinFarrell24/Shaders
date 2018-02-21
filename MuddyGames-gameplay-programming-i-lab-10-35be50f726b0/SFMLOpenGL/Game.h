#ifndef GAME_H
#define GAME_H

#include <Debug.h>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <Vector3.h>
#include <Matrix3.h>

using namespace std;
using namespace sf;
//using namespace gpp;

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	Window window;
	bool isRunning = false;
	void initialize();

	void initializeFrontBackFaces();
	void initializeTopBottomFaces();
	void initializeLeftRightFaces();

	void update();
	void render();
	void unload();

	Clock clock;
	Time elapsed;

	float rotationAngleX = 0.0f;
	float rotationAngleY = 0.0f;

	float transitionFactorX = 0.0f;
	float transitionFactorY = 0.0f;

	float scaleFactor = 100;

	Matrix3 m_rotationXMatrix;
	Matrix3 m_rotationYMatrix;

	Matrix3 m_translationMatrix;
	Matrix3 m_scaleMatrix;

	Vector3D m_startPoints[36];
	Vector3D m_currentPoints[36];
};

#endif