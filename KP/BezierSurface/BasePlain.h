#include "Matrix.h"

class BasePlain {
public:
	BasePlain(Vector topLeft, GLfloat w, GLfloat h);
	~BasePlain();
	void draw(Matrix& cm);
private:
	std::vector<Vector> points;
};