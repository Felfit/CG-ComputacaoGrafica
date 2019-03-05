#include "Model3D.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include "sceneParser.h"
#include "Model3D.h"
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

float alfaC;
float betaC;
float radiusC = 10;
Scene s;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	float x = radiusC * sin(alfaC) * cos(betaC);
	float y = radiusC * sin(betaC);
	float z = radiusC * cos(alfaC) * cos(betaC);

	gluLookAt(x, y, z,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
		
	for (const auto& model : s->objetos) {
		s->modelos[model]->drawModel();
	}

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events


void keyboard_func(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':
		radiusC += 1;
		renderScene();
		break;
	case '2':
		radiusC -= 1;
		renderScene();
		break;
	case '3':
		glPolygonMode(GL_FRONT, GL_FILL);
		renderScene();
		break;
	case '4':
		glPolygonMode(GL_FRONT, GL_LINE);
		renderScene();
		break;
	case '5':
		glPolygonMode(GL_FRONT, GL_POINT);
		renderScene();
		break;
	case '6':
		glCullFace(GL_FRONT);
		renderScene();
		break;
	case '7':
		glCullFace(GL_BACK);
		renderScene();
		break;
	default:
		break;
	}
}

void special_func(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		betaC += M_PI / 8;
		renderScene();
		break;
	case GLUT_KEY_DOWN:
		betaC -= M_PI / 8;
		renderScene();
		break;
	case GLUT_KEY_LEFT:
		alfaC += M_PI / 8;
		renderScene();
		break;
	case GLUT_KEY_RIGHT:
		alfaC -= M_PI / 8;
		renderScene();
		break;
	default:
		break;
	}
}

int main(int argc, char **argv) {
	
	if (argc <= 1) {
		fputs("Usage: engine <config>\n", stdout);
		return 1;
	}
	s = new struct scene;
	sceneParser(argv[1], s);

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Engine");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboard_func);
	glutSpecialFunc(special_func);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
