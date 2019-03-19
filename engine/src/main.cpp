#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "Scene.h"
#include "Model3D.h"

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
Scene scene;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

float theta = 0.03141593*2;
float camAlpha = -1.822124;
float camBeta = -0.251327;
float camx = 45.267159;
float camy = 41.017670;
float camz = 113.516701;
int speed = 2;

void drawCamera() {
	double lookatxyz[3] = {
		cos(camBeta)*cos(camAlpha),
		sin(camBeta),
		sin(camAlpha)
	};

	gluLookAt(camx, camy, camz,
		lookatxyz[0] + camx, lookatxyz[1] + camy, lookatxyz[2] + camz,
		0, 1, 0);
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
	drawCamera();
	
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);
	//glColor3f(0, 1, 0);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	//glColor3f(0, 0, 1);
	glVertex3f(0, 0, 100);
	glVertex3f(0, 0, -100);
	glEnd();

	scene.draw();
	
	// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int keycode, int x, int y) {
	switch (keycode)
	{
	case GLUT_KEY_DOWN:
		camy -= speed;
		break;
	case GLUT_KEY_UP:
		camy += speed;
		break;
	}
	renderScene();
}

void processKeys(unsigned char keycode, int x, int y) {
	switch (keycode)
	{
	case 'q':
		camAlpha -= theta;
		break;
	case 'e':
		camAlpha += theta;
		break;
	case 'w':
		camx += speed*cos(camBeta)*cos(camAlpha);
		camy += speed * sin(camBeta);
		camz += speed * sin(camAlpha);
		break;
	case 's':
		camx -= speed * cos(camBeta)*cos(camAlpha);
		camy += speed * sin(camBeta+M_PI);
		camz-= speed * sin(camAlpha);
		break;
	case 'a':
		camx += speed * cos(camBeta)*cos(camAlpha - M_PI_2);
		camz += speed * sin(camAlpha - M_PI_2);
		break;
	case 'd':
		camx += speed * cos(camBeta)*cos(camAlpha + M_PI_2);
		camz += speed * sin(camAlpha + M_PI_2);
		break;
	case 'f':
		if(theta >-M_2_PI)
			camBeta -= theta;
		break;
	case 'r':
		if (theta < M_2_PI)
			camBeta += theta;
		break;
	case '1':
		glPolygonMode(GL_FRONT, GL_FILL);
		renderScene();
		break;
	case '2':
		glPolygonMode(GL_FRONT, GL_LINE);
		renderScene();
		break;
	case '3':
		glPolygonMode(GL_FRONT, GL_POINT);
		renderScene();
		break;
	default:
		return;
		break;
	}
	renderScene();
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse WASD to move the camera, up/down to raise the camera, QERF to rotate the camera \n");
	printf("Use 1:FillMode 2:Wireframe 3:Point Mode\n");
}


int main(int argc, char **argv) {
	/*
	if (argc <= 1) {
		fputs("Usage: engine <config>\n", stdout);
		return 1;
	}
	s.parse(argv[2]);
	*/



// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("engine");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	spherical2Cartesian();
	printInfo();
	//Load de coisas
	scene.parse("exemplo.xml");

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
