#include <vector>
#include <GLFW/glfw3.h>
#include "Vector.h"
#include "Matrix.h"

class BSurface {
public:
	BSurface(std::vector<Vector*>& ctrls);
	~BSurface();
	void draw(Matrix& cm);

	bool isEnabled();
	void enable();
	void setCtrls(std::vector<Vector*>& ctrls);

	Vector p;

private:
	std::vector<Vector*> controlpoints;
	bool enabled = false;

	void surfacePoint(GLfloat u, GLfloat v, Vector* sum);
	GLfloat bernstein(GLfloat n, GLfloat i, GLfloat u);
	GLfloat binom(GLfloat n, GLfloat i);
	GLfloat fact(GLint i);
};