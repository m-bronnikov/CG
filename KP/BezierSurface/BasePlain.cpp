 #include "BasePlain.h"

BasePlain::BasePlain(Vector topLeft, GLfloat w, GLfloat h)
{
	points.push_back(topLeft);
	points.push_back(topLeft + Vector(w, 0, 0, 0));
	points.push_back(topLeft + Vector(w, 0, h, 0));
	points.push_back(topLeft + Vector(0, 0, h, 0));
  
}

BasePlain::~BasePlain()
{

}

void BasePlain::draw(Matrix& cm)
{
	glBegin(GL_LINE_LOOP);
	for (GLint i = 0; i < 4; ++i) {
		Vector p = cm * points.at(i);

		glVertex2f(p[0], p[1]);
	}
	glEnd();
}



