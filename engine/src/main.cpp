#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <ATB/AntTweakBar.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "Scene.h"
#include "Model3D.h"
#include "main.h"

Scene scene;


// tracking do rato
int startX, startY, tracking = 0; 
constexpr auto MOVE = 1;
constexpr auto ZOOM = 2;

// tamanho inicial da janela
constexpr auto height = 800, width = 800;

// camara quando controlada por rato
int Malpha = 0, Mbeta = 0, Mr = 100; 

// camara quando controlada por teclado
int alpha = 0, beta = -25, theta = 2.5, r = 50, speed = 2;

// camara
float centerX = 0;
float centerY = 0;
float centerZ = 0;
float eyeX = Mr * sin(Malpha * 3.14 / 180.0) * cos(Malpha * 3.14 / 180.0);
float eyeZ = Mr * cos(Malpha * 3.14 / 180.0) * cos(Malpha * 3.14 / 180.0);
float eyeY = Mr * sin(Malpha * 3.14 / 180.0);

int camerafollow = -1;

bool isDrawingAxis = false;
bool isLightingEnabled = true;
GLenum currentPolyMode = GL_FILL;


void placeCamera() {
	float camAlpha = alpha * M_PI / 180;
	float camBeta = beta * M_PI / 180;

	
	
	float upY = sin(camBeta + M_PI_2);
	

	gluLookAt(centerX+eyeX, centerY+eyeY, centerZ+eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
	/*
	gluLookAt(eyeX, eyeY, eyeZ,
			  centerX, centerY, centerZ,
		      0.0f, upY, 0.0f);
	*/
}




void reshape(int w, int h) {

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

	TwWindowSize(w, h);
}

void drawAxis() {
	glBegin(GL_LINES);
	float red[4] = { 1, 0, 0, 1 };
	glMaterialfv(GL_FRONT, GL_EMISSION, red);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glColor3f(1, 0, 0);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);

	float green[4] = { 0, 1, 0, 1 };
	glMaterialfv(GL_FRONT, GL_EMISSION, green);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glColor3f(0, 1, 0);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);

	float blue[4] = { 0, 0, 1, 1 };
	glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 100);
	glVertex3f(0, 0, -100);
	glEnd();

	glColor3f(1, 1, 1);
}

void display(void) {

	// clear buffers
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Mete camera a seguir modelo se esta estiver a seguir modelo
	scene.followModel();
	// set the camera
	
	glLoadIdentity();
	placeCamera();
	scene.drawSkybox(centerX,centerY,centerZ);

	// XYZ axis
	if (isDrawingAxis) {
		drawAxis();
	}

	scene.draw();

	TwDraw();
	
	
	glutSwapBuffers();

	glutPostRedisplay();
}

unsigned char picking(int x,int y) {
	GLint viewport[4];
	unsigned char res[4];
	bool lightingstate = glIsEnabled(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//Camera
	placeCamera();
	glDepthFunc(GL_LEQUAL);
	//DrawColor
	scene.drawColor();

	//EndDrawColor
	glDepthFunc(GL_LESS);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	return res[0];
}


void processSpecialKeys(int keycode, int x, int y) {
	switch (keycode) {
	case GLUT_KEY_DOWN:
		centerY -= speed;
		break;
	case GLUT_KEY_UP:
		centerY += speed;
		break;
	}
	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (!TwEventMouseButtonGLUT(button, state, xx, yy))
	{
		if (state == GLUT_DOWN) {
			startX = xx;
			startY = yy;
			if (button == GLUT_LEFT_BUTTON)
				tracking = MOVE;
			else if (button == GLUT_RIGHT_BUTTON)
				tracking = ZOOM;
			else 
				tracking = 0;
			if (button == GLUT_MIDDLE_BUTTON) {
				camerafollow = picking(xx, yy);
				printf("Hitting modelnumber: %d\n", camerafollow);
			}
		}
		else if (state == GLUT_UP) {
			if (tracking == MOVE) {
				Malpha += (xx - startX);
				Mbeta += (yy - startY);
			}
			else if (tracking == ZOOM) {

				Mr -= yy - startY;
				if (Mr < 3)
					Mr = 3.0;
			}
			tracking = 0;
		}
	}
}

void processMouseMotion(int xx, int yy)
{
	if (!TwEventMouseMotionGLUT(xx, yy))
	{
		if (!tracking)
			return;

		int deltaX = xx - startX;
		int deltaY = yy - startY;

		int alphaAux = Malpha;
		int betaAux = Mbeta;
		int rAux = Mr;

		if (tracking == MOVE) {

			alphaAux += deltaX;
			betaAux += deltaY;

			if (betaAux > 85.0)
				betaAux = 85.0;
			else if (betaAux < -85.0)
				betaAux = -85.0;

		}
		else if (tracking == ZOOM) {

			rAux -= deltaY;
			if (rAux < 3)
				rAux = 3;
		}

		eyeX = rAux * sin(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
		eyeZ = rAux * cos(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
		eyeY = rAux * sin(betaAux * M_PI / 180.0);

		glutPostRedisplay();
	}
}

void processKeys(unsigned char keycode, int x, int y) {
	if (!TwEventKeyboardGLUT(keycode, x, y))  // send event to AntTweakBar
	{ 
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
			centerX += dx;
			centerZ += dz;
			break;
		case 's':
			centerX -= dx;
			centerZ -= dz;
			break;
		case 'a':
			centerX -= rx;
			centerZ -= rz;
			break;
		case 'd':
			centerX += rx;
			centerZ += rz;
			break;
		case 'f':
			if (beta > -60)
				beta -= theta;
			break;
		case 'r':
			if (beta < 60)
				beta += theta;
			break;
		default:
			break;
		}

	}
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse WASD to move the camera, up/down to raise the camera, QERF to rotate the camera \n"); // TODO: atualizar
	printf("Use mouse middle button to open menu.\n");
}


void openGLInit() {

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

// Function called at exit
void Terminate(void)
{
	TwTerminate();
}

void TW_CALL setLighting(const void* value, void* clientData)
{
	isLightingEnabled = *(const bool*)value;
	if (isLightingEnabled) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
}

void TW_CALL getLighting(void* value, void* clientData)
{
	*(bool*)value = isLightingEnabled;
}

void TW_CALL setPolyMode(const void* value, void* clientData)
{
	currentPolyMode = *(const GLenum*)value;
	glPolygonMode(GL_FRONT, currentPolyMode);
}

void TW_CALL getPolyMode(void* value, void* clientData)
{
	*(GLenum*)value = currentPolyMode;
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
	glutInitWindowSize(width, height);
	glutCreateWindow("engine");
		
	// Required callback registry 
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(NULL);

	// Callback registration
	atexit(Terminate);
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion
	TwGLUTModifiersFunc(glutGetModifiers);
	/*
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	*/
	
	// TweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);

	TwBar* bar;
	bar = TwNewBar("TweakBar");
	TwDefine(" TweakBar size='200 100' color='96 216 224' "); // tamanho e cor iniciais

	TwAddVarRW(bar, "Draw Axis", TW_TYPE_BOOLCPP, &isDrawingAxis, "  key=x  ");
	TwAddVarCB(bar, "Lighting", TW_TYPE_BOOLCPP, setLighting, getLighting, NULL, "  key=l  true='Enabled' false='Disabled' ");

	TwEnumVal polyModeEV[3] = { {GL_FILL, "Fill"}, {GL_LINE, "Line"}, {GL_POINT, "Point"} };
	TwType polyModeType = TwDefineEnum("PolygonModeType", polyModeEV, 3);
	TwAddVarCB(bar, "Polygon Mode", polyModeType, setPolyMode, getPolyMode, NULL, " keyincr=f ");


#ifndef __APPLE__	
	glewInit();
#endif	

	ilInit();

	openGLInit();

	printInfo();

	//	Load scene
	scene.parse(argv[1]);

	glutMainLoop();

	return 1;
}
