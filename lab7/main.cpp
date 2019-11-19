// Made by Max Bronnikov

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <math.h>


// compile: g++ -std=c++11 main.cpp -o main -lglut -lGL -lGLU -lglfw


using namespace std;
using namespace glm;

#define PI		3.14159265359
#define TWOPI	PI*2

GLFWwindow *window;
int w, h;
double mouseX, mouseY;

vector<dvec2> control;		// контрольные точки вектора
float cRadius = 0.0135f;		// радиус контрольной точки(для захвата мышкой)
int selected = -1;			// индекс захваченной точки
bool movePoint = false;		// точка перемещается
bool showPoints = true;		// показывается контрольная точка
bool showColours = true;	// показывать цвета
bool niceLines = true;	


double uinc = 0.006;	// Апроксимация

bool rotating = false;		// вращается
float yangle = 0.f;			// угол вращения вокруг y
float zangle = 0.f;			// угол вращения вокруг z 
float rotSpeed = 100.f;		// скорость вращения
float scale = 1.f;			// параметр масштабирования
float zoomSpeed = 0.02f;

double B0(double u){
	return ((1.0 - u) * (1.0 - u) * (1.0 - u)) / 6.0;
}

double B1(double u){
	return (3.0*u*u*u - 6.0*u*u + 4.0) / 6.0;
}

double B2(double u){
	return (-3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0) / 6.0;
}

double B3(double u){
	return (u * u * u) / 6.0;
}



// Функция отрисовки
void render() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Задаем значения для инициализации в библиотеке
	if (niceLines) {
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
	}

	// Добляем к матрице модели масштабирование и повороты вокруг осей z и y
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(scale, scale, scale);			// scale by factor
	glRotatef(yangle, 0.0f, 1.0f, 0.0f);	// rotate by y angle
	glRotatef(zangle, 0.0f, 0.0f, 1.0f);	// rotate by z angle

	// Задаем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -10, 10);

	// Отрисовываем контрольные точки в виде квадратиков
	if (showPoints) {
		glBegin(GL_QUADS);

		for (int i = 0; i < control.size(); i++) {
			float pr = 1.f;
			float pg = 1.f;
			float pb = 1.f;
			if (selected == i) {
				pg = 0.f;
				pb = 0.f;
			}

			glColor3f(pr, pg, pb);
			glVertex3f(control[i].x + cRadius, control[i].y + cRadius, 0);
			glColor3f(pr, pg, pb);
			glVertex3f(control[i].x + cRadius, control[i].y - cRadius, 0);
			glColor3f(pr, pg, pb);
			glVertex3f(control[i].x - cRadius, control[i].y - cRadius, 0);
			glColor3f(pr, pg, pb);
			glVertex3f(control[i].x - cRadius, control[i].y + cRadius, 0);
		}

		glEnd();
	}

	// Отрисовываем кривую
	glLineWidth(3.5);
	glBegin(GL_LINE_STRIP);

	// генерируем B-spline
	// Иттерация 
	float cr = 1.0;
	float cg = 0.2;
	float cb = 0.3;
	int size = (int)control.size();
	for(int i = 0; i < size - 3; ++i){
		for(double u = 0; u <= 1.0; u+=uinc){
			glColor3f(cr, cg, cb);
			dvec2 point = B0(u)*control[i] + B1(u)*control[i+1] +
						B2(u)*control[i+2] + B3(u)*control[i+3];
			glVertex3f(point.x, point.y, 0.0);
		}
	}
	/*if(size >= 3){
		for(double u = 0; u <= 1.0; u+=uinc){
			glColor3f(cr, cg, cb);
			dvec2 point = B0(u)*control[size-3] + B1(u)*control[size-2] +
						B2(u)*control[size-1];
			glVertex3f(point.x, point.y, 0.0);
		}
	}
	if(size >= 2){
		for(double u = 0; u <= 1.0; u+=uinc){
			glColor3f(cr, cg, cb);
			dvec2 point = B0(u)*control[size - 2] + B1(u)*control[size-1];
			glVertex3f(point.x, point.y, 0.0);
		}
	}
	if(size >= 1){
		for(double u = 0; u <= 1.0; u+=uinc){
			glColor3f(cr, cg, cb);
			dvec2 point = B0(u)*control[size-1];
			glVertex3f(point.x, point.y, 0.0);
		}
	}*/

	glEnd();
}

// Обработка нажатий клавиатуры
void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Настройка аппроксимация

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		uinc *= 0.9;

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		uinc *= 1.1;

	if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
		showPoints = !showPoints;

	if (key == GLFW_KEY_END && action == GLFW_PRESS)
		niceLines = !niceLines;

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
		showColours = !showColours;

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		scale = 1.f;
		yangle = 0.f;
		zangle = 0.f;
	}

	// обнуление(возвращение в дефолт)
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		control.clear();
		//k = 2;
		uinc = 0.006;
		showPoints = true;
		scale = 1.f;
		yangle = 0.f;
		zangle = 0.f;
	}
}

// Обработка нажатий мыши
void mouseClick (GLFWwindow *sender, int button, int action, int mods) {

    if (action == GLFW_PRESS) {
		selected = -1;
		double x = (2 * mouseX / w) - 1;
		double y = (-2 * mouseY / h) + 1;

		// Выбор точки для перемещения
		for (int i = 0; i < control.size(); i++) {
			if (abs(control[i].x - x) <= cRadius && abs(control[i].y - y) <= cRadius) {
				selected = i;
				cout << "Selected point: " << selected << endl;
				movePoint = true;
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && selected == -1) {
			// Создание новой контрольной точки
			cout << "Added point: " << control.size() << endl;
			control.push_back(vec2(x, y));
		}

		else if (button == GLFW_MOUSE_BUTTON_RIGHT && selected >= 0) {
			// Удаление старой контрольной точки
			control.erase(control.begin() + selected);
			cout << "Deleted point: " << selected << endl;
			selected = -1;
		}

		else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			glfwGetCursorPos(window, &mouseX, &mouseY);
			rotating = true;
		}
    }

	if (action == GLFW_RELEASE) {
		movePoint = false;
		rotating = false;
	}
}

// Обработка координат мыши

void mousePos(GLFWwindow *sender, double x, double y) {
	// вращение
	if (rotating) {
		yangle += rotSpeed * float(x - mouseX) / float(w);
		zangle += rotSpeed * float(y - mouseY) / float(h);
	}

	mouseX = x;
	mouseY = y;

	// перемещение выбранной точки
	if (movePoint && selected >= 0) {
		double newx = (2 * mouseX / w) - 1;
		double newy = (-2 * mouseY / h) + 1;
		control[selected] = vec2(newx, newy);
	}
}


void mouseScroll(GLFWwindow *sender, double x, double y) {
	// масштабирование
	scale += zoomSpeed * y;
}


int main () {
	// инициализация GLFW
	if (!glfwInit()) return 1;

	// создание окна
	window = glfwCreateWindow (768, 768, "Lab 7-12", NULL, NULL);
	if (!window) return 1;
	glfwMakeContextCurrent (window);

	// функции обратного вызова
	glfwSetKeyCallback (window, keyboard);
	glfwSetMouseButtonCallback (window, mouseClick);
	glfwSetCursorPosCallback (window, mousePos);
	glfwSetScrollCallback(window, mouseScroll);

	// игровой цикл
	while (!glfwWindowShouldClose (window)) {
		glfwGetFramebufferSize (window, &w, &h);
		glViewport (0, 0, w, h);

		render ();

		glfwSwapBuffers (window);
		glfwPollEvents();
	}

	// очистка окна и выход
	glfwDestroyWindow (window);
	glfwTerminate();
	return 0;
}


