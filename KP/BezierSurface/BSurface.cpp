#include "BSurface.h"

BSurface::BSurface(std::vector<Vector*>& ctrls) {

	controlpoints = ctrls;

}

BSurface::~BSurface() {
	
}

void BSurface::draw(Matrix& cm) {
	
	if (enabled) {
		Vector* sum = new Vector();

		glColor3f(0.f, 0.f, 1.f);

		for (double u = 0; u < 1.0; u += 0.05)
		{
			glBegin(GL_LINE_STRIP);
			for (double v = 0; v < 1.0; v += 0.05)
			{
				
				sum->setNull();
				surfacePoint(u, v, sum);
				
				cm.destructMultiply(sum);

				glVertex2f(sum->coords[0], sum->coords[1]);
				
			}
			glEnd();
		}

		for (double v = 0; v < 1.0; v += 0.05)
		{
			glBegin(GL_LINE_STRIP);
			for (double u = 0; u < 1.0; u += 0.05)
			{
				sum->setNull();
				surfacePoint(u, v, sum);
				Vector p = cm * *sum;

				glVertex2f(p[0], p[1]);
			}
			glEnd();
		}
		
		delete sum;
	}
}

bool BSurface::isEnabled()
{
	return enabled;
}

void BSurface::enable() {
	enabled = true;
}

void BSurface::setCtrls(std::vector<Vector*>& ctrls)
{
	controlpoints = ctrls;

	for (Vector* p : ctrls) {
		std::cout << *p << std::endl;
	}
}

void BSurface::surfacePoint(GLfloat u, GLfloat v, Vector* sum) {
	
	for (GLint i = 0; i < 4; ++i) {
		for (GLint j = 0; j < 4; ++j) {
			
			p[0] = controlpoints[i * 4 + j]->coords[0];
			p[1] = controlpoints[i * 4 + j]->coords[1];
			p[2] = controlpoints[i * 4 + j]->coords[2];
			p[3] = controlpoints[i * 4 + j]->coords[3];

			p.destructMultiply(bernstein(3.f, i, u) * bernstein(3.f, j, v));

			sum->coords[0] += p[0];
			sum->coords[1] += p[1];
			sum->coords[2] += p[2];
		}
	}
}

GLfloat BSurface::bernstein(GLfloat n, GLfloat i, GLfloat u)
{
	
	return binom(n, i) * std::pow(u, i) * std::pow(1.0-u, n-i);

}

GLfloat BSurface::binom(GLfloat n, GLfloat i) {
	return fact(n)/ (fact(i)*fact(n-i));
}

GLfloat BSurface::fact(GLint n) {
	
	GLint res = 1;

	for (GLint i = n; i > 1; --i)
	{
		res *= i;
	}

	return res;

}