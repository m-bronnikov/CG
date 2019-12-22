#pragma once
#include "BasePlain.h"
#include "BSurface.h"

class BezierSurface {

public:
	constexpr static GLint SCREEN_WIDTH = 1280;
	constexpr static GLint SCREEN_HEIGHT = 720;
	
	static const GLfloat PI;

	static GLint clicked;

	static Matrix cameraMatrix, reverseMatrix, wtvMatrix;
	static BasePlain clickPlain;
	static BSurface bezier;
	static std::vector<Vector*> ctrlpointsPlain;
	static std::vector<Vector*> ctrlpointsReal;

	~BezierSurface();

	void static init();
	void static renderPoints();
	void static renderNet();
};