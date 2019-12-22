#include "BezierSurface.h"

const GLfloat BezierSurface::PI = 3.1415926539;
GLint BezierSurface::clicked = -1;

BasePlain BezierSurface::clickPlain = BasePlain(Vector(100, 100), 700, 700);
BSurface BezierSurface::bezier = BSurface(BezierSurface::ctrlpointsReal);

Matrix BezierSurface::cameraMatrix = Matrix({
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ (float)(1.0 / 2.0 * std::cos(45 * (BezierSurface::PI / 180.0))), (float)(1.0 / 2.0 * std::sin(45 * (BezierSurface::PI / 180.0))), 0, 0 },
	{ 0, 0, 0, 1 }
});

Matrix BezierSurface::reverseMatrix = Matrix({
	{ 1, 0, 0, 0 },
	{ (float)(-std::cos(45 * (BezierSurface::PI / 180.0)) / std::sin(45 * (BezierSurface::PI / 180.0))), 0, (float)(2.0/std::sin(45 * (BezierSurface::PI / 180.0))), 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 1 }
});

std::vector<Vector*> BezierSurface::ctrlpointsPlain = std::vector<Vector*>();
std::vector<Vector*> BezierSurface::ctrlpointsReal = std::vector<Vector*>();

BezierSurface::~BezierSurface() {
	for (Vector* p : ctrlpointsPlain) {
		delete p;
	}

	for (Vector* p : ctrlpointsReal) {
		delete p;
	}
}

void BezierSurface::init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, BezierSurface::SCREEN_WIDTH, 0.f, BezierSurface::SCREEN_HEIGHT, 0.f, 1.f);
	glEnable(GL_POINT_SMOOTH);

}

void BezierSurface::renderPoints()
{
	glPointSize(5.0);
	glColor3f(0, 1, 0);
	glBegin(GL_POINTS);
	for (Vector* plainPoint : BezierSurface::ctrlpointsPlain) {
		Vector p =  cameraMatrix * (*plainPoint);

		
		glVertex2f( p[0], p[1] );
	}
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (Vector* realPoint : BezierSurface::ctrlpointsReal) {
		Vector p =  cameraMatrix * (*realPoint);

		glVertex2f( p[0], p[1] );
	}
	glEnd();
}

void BezierSurface::renderNet()
{
	if (ctrlpointsReal.size() < 16) {
		return;
	}

	glColor3f(0, 0, 0);

	glBegin(GL_LINE_STRIP);
	GLint u = 1;
	for (GLint i = 0; i < ctrlpointsReal.size(); i+=u) {
		Vector p = cameraMatrix * (*ctrlpointsReal[i]);

		glVertex2f(p[0], p[1]);

		if (u > 0 && i % 4 == 3) {
			u = -1;
			i += 5;
		} else if (u < 0 && i % 4 == 0) {
			u = 1;
			i += 3;
		}

	}
	
	glEnd();
/*
	glBegin(GL_LINES);
	for (GLint i = 0; i < ctrlpointsReal.size(); ++i) {
		if (i + 4 < 16 ) {
			Vector p = cameraMatrix * (*ctrlpointsReal[i]);
			Vector o = cameraMatrix * (*ctrlpointsReal[i+4]);

			glVertex2f(p[0], p[1]);
			glVertex2f(o[0], o[1]);
		}
	}
	glEnd(); */
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "cl: " << BezierSurface::clicked << std::endl;
	BezierSurface::clicked = -1;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = std::fabs(BezierSurface::SCREEN_HEIGHT - ypos);
		std::cout << xpos << ", " << ypos << std::endl;
		
		Vector projPoint =  Vector(xpos, ypos);

		for (GLint i = 0; i < BezierSurface::ctrlpointsReal.size(); ++i) {
			Vector cp = BezierSurface::cameraMatrix * *BezierSurface::ctrlpointsReal.at(i);
			if (std::fabs(cp[0] - projPoint[0]) < 5 && std::fabs(cp[1] - projPoint[1]) < 5) {
				BezierSurface::clicked = 16 + i;
				std::cout << "real cl: " << BezierSurface::clicked << std::endl;
				return;
			}
		}

		for (GLint i = 0; i < BezierSurface::ctrlpointsPlain.size(); ++i) {
			Vector cp = BezierSurface::cameraMatrix * *BezierSurface::ctrlpointsPlain.at(i);
			if (std::fabs(cp[0] - projPoint[0]) < 5 && std::fabs(cp[1] - projPoint[1]) < 5) {
				BezierSurface::clicked =  i;
				std::cout << "plain cl: " << BezierSurface::clicked << std::endl;
				return;
			}
		}
		
		if (BezierSurface::ctrlpointsPlain.size() < 16) {

			Vector p = BezierSurface::reverseMatrix * Vector(xpos, ypos);

			std::cout << p << std::endl;

			BezierSurface::ctrlpointsPlain.push_back(new Vector(p));
			BezierSurface::ctrlpointsReal.push_back(new Vector(p));

			if (BezierSurface::ctrlpointsPlain.size() == 16) {
				//BezierSurface::bezier = BSurface();

				BezierSurface::bezier.setCtrls(BezierSurface::ctrlpointsReal);
				BezierSurface::bezier.enable();
			}
		}
		
	}
		
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (BezierSurface::clicked != -1) {
		ypos = std::fabs(BezierSurface::SCREEN_HEIGHT - ypos);  

		if (BezierSurface::clicked / 16 == 0) {
			Vector p = BezierSurface::reverseMatrix * Vector(xpos, ypos);

			(*BezierSurface::ctrlpointsPlain[BezierSurface::clicked % 16])[0] = p[0];
			(*BezierSurface::ctrlpointsPlain[BezierSurface::clicked % 16])[2] = p[2];

			(*BezierSurface::ctrlpointsReal[BezierSurface::clicked % 16])[0] = p[0];
			(*BezierSurface::ctrlpointsReal[BezierSurface::clicked % 16])[2] = p[2];
		}
		else {
			(*BezierSurface::ctrlpointsReal[BezierSurface::clicked % 16])[1] = (ypos - 1.0 / 2.0 * std::sin(45 * (BezierSurface::PI / 180.0)) * (*BezierSurface::ctrlpointsPlain[BezierSurface::clicked % 16])[2]);

		}

	}
}

int main() {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(BezierSurface::SCREEN_WIDTH, BezierSurface::SCREEN_HEIGHT, "BezierSurface", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	/* -- Init -- */
	BezierSurface::init();
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		/* -- Render -- */
		BezierSurface::clickPlain.draw( BezierSurface::cameraMatrix);
		BezierSurface::bezier.draw(BezierSurface::cameraMatrix);
		BezierSurface::renderNet();
		BezierSurface::renderPoints();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	
	/* -- Terminate -- */

	return 0;
}