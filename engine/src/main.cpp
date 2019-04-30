#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "Scene.h"
#include "Model3D.h"

Scene scene;


float eyeX = -20;
float eyeY = 100;
float eyeZ = 0;
int speed = 2;

int alpha = 0, beta = -25, theta = 2.5, r = 50;

bool drawAxis = false;

void placeCamera() {
	float camAlpha = alpha * M_PI / 180;
	float camBeta = beta * M_PI / 180;

	float centerX = eyeX + cos(camBeta) * cos(camAlpha);
	float centerY = eyeY + sin(camBeta);
	float centerZ = eyeZ + sin(camAlpha);

	float upY = sin(camBeta + M_PI_2);

	gluLookAt(eyeX, eyeY, eyeZ,
			  centerX, centerY, centerZ,
		      0.0f, upY, 0.0f);
}

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

	placeCamera();
	// XYZ axis
	if (drawAxis) {
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-100, 0, 0);
		glVertex3f(100, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, -100, 0);
		glVertex3f(0, 100, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 100);
		glVertex3f(0, 0, -100);
		glEnd();
	}

	glColor3f(1, 1, 1);
	scene.draw();
	
	// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int keycode, int x, int y) {
	switch (keycode) {
	case GLUT_KEY_DOWN:
		eyeY -= speed;
		break;
	case GLUT_KEY_UP:
		eyeY += speed;
		break;
	}
	glutPostRedisplay();
}

void processKeys(unsigned char keycode, int x, int y) {
	float camAlpha = alpha * M_PI / 180.0;
	float camBeta = beta * M_PI / 180.0;
	float dx = speed * cos(camAlpha);
	float dz = speed * sin(camAlpha);
	float rx = speed * cos(camAlpha + M_PI_2);
	float rz = speed * sin(camAlpha + M_PI_2);
	switch (keycode) {
		case 'q':
			alpha -= theta;
			break;
		case 'e':
			alpha += theta;
			break;
		case 'w':
			eyeX += dx;
			eyeZ += dz;
			break;
		case 's':
			eyeX -= dx;
			eyeZ -= dz;
			break;
		case 'a':
			eyeX -= rx;
			eyeZ -= rz;
			break;
		case 'd':
			eyeX += rx;
			eyeZ += rz;
			break;
		case 'f':
			if(beta > -60)
				beta -= theta;
			break;
		case 'r':
			if (beta < 60)
				beta += theta;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse WASD to move the camera, up/down to raise the camera, QERF to rotate the camera \n");
	printf("Use mouse rightclick to open menu.\n");
}




int startX, startY, tracking = 0;
int Malpha = 0, Mbeta = 0, Mr = 5;

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			Malpha += (xx - startX);
			Mbeta += (yy - startY);
		}
		else if (tracking == 2) {

			Mr -= yy - startY;
			if (Mr < 3)
				Mr = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	int deltaX = xx - startX;
	int deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = Malpha + deltaX;
		betaAux = Mbeta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = Mr;
	}
	else if (tracking == 2) {

		alphaAux = Malpha;
		betaAux = Mbeta;
		rAux = Mr - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	eyeX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	eyeZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	eyeY = rAux * sin(betaAux * 3.14 / 180.0);
	glutPostRedisplay();
}



void processMenuEvents(int op) {
	switch (op) {
	case 1:
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 2:
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 3:
		glPolygonMode(GL_FRONT, GL_POINT);
		break;
	case 4:
		drawAxis = true;
		break;
	case 5:
		drawAxis = false;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void createMenu() {
	int polygonModeMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("fill", 1);
	glutAddMenuEntry("line", 2);
	glutAddMenuEntry("point", 3);

	int axisMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("show", 4);
	glutAddMenuEntry("hide", 5);

	glutCreateMenu(NULL);
	glutAddSubMenu("Polygon Mode", polygonModeMenu);
	glutAddSubMenu("XYZ axis", axisMenu);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}


void initOpenGL() {

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	

	//glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // ver luses?

}

int main(int argc, char **argv) {
	
	if (argc <= 1) {
		fputs("Usage: engine <config filename>\n", stdout);
		return 1;
	}
	
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("engine");
		
	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(NULL);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	createMenu();

#ifndef __APPLE__	
	glewInit();
#endif	

	initOpenGL();

	printInfo();

	//	Load scene
	scene.parse(argv[1]);

	glutMainLoop();
	
	return 1;
}
