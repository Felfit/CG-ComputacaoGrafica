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
#include "utils.hpp"

Scene scene;

// tracking do rato
int startX, startY, tracking = 0; 
constexpr auto MOVE = 1;
constexpr auto ZOOM = 2;


// tamanho inicial da janela
constexpr auto height = 800, width = 800;


// camara 
int alpha = 0, beta = 0, r = 100; 

Point3D center = Point3D(0, 0, 0);
float eyeX = r * sin(alpha * 3.14 / 180.0) * cos(alpha * 3.14 / 180.0);
float eyeZ = r * cos(alpha * 3.14 / 180.0) * cos(alpha * 3.14 / 180.0);
float eyeY = r * sin(alpha * 3.14 / 180.0);

int camerafollow = 0; // id do modelo que está a ser seguido, se 0 segue centro da cena


// definições gerais
bool isDrawingAxis = false;
bool isLightingEnabled = true;
GLenum currentPolyMode = GL_FILL;



void placeCamera() {
	gluLookAt(	center.x + eyeX, center.y + eyeY, center.z + eyeZ, 
				center.x, center.y, center.z, 
				0.0f, 1.0f, 0.0f	);
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
	glDisable(GL_LIGHTING);

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

	if (isLightingEnabled) glEnable(GL_LIGHTING);

	glColor3f(1, 1, 1);
}

void display(void) {

	// clear buffers
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// mete camera a seguir modelo se esta estiver a seguir modelo
	scene.followModel(camerafollow, &center);

	// set the camera
	
	glLoadIdentity();
	placeCamera();

	scene.drawSkybox(center.x, center.y, center.z);

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
				if (!camerafollow) {
					center.x = 0;
					center.y = 0;
					center.z = 0;
				}
				glutPostRedisplay();
				printf("Hitting modelnumber: %d, %f %f %f\n", camerafollow, center.x, center.y, center.z);
			}
		}
		else if (state == GLUT_UP) {
			if (tracking == MOVE) {
				alpha += (xx - startX);
				beta += (yy - startY);
			}
			else if (tracking == ZOOM) {

				r -= yy - startY;
				if (r < 3)
					r = 3.0;
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

		int alphaAux = alpha;
		int betaAux = beta;
		int rAux = r;

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

		
	}
	glutPostRedisplay();
}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse mouse left clik drag to move the camera, right click drag up/down to zoom out/in, middle button a model to focus the camera on it.\n");
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
void Terminate()
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
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	

	// TweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);

	TwBar* bar;
	bar = TwNewBar("TweakBar");
	TwDefine(" TweakBar size='300 400' color='96 216 224' refresh=0.5 iconified=false ");

	TwAddVarRW(bar, "Draw Axis", TW_TYPE_BOOLCPP, &isDrawingAxis, " group='Display' key=x  ");
	TwAddVarCB(bar, "Lighting", TW_TYPE_BOOLCPP, setLighting, getLighting, NULL, " group='Display' key=l  true='Enabled' false='Disabled' ");

	TwEnumVal polyModeEV[3] = { {GL_FILL, "Fill"}, {GL_LINE, "Line"}, {GL_POINT, "Point"} };
	TwType polyModeType = TwDefineEnum("PolygonModeType", polyModeEV, 3);
	TwAddVarCB(bar, "Polygon Mode", polyModeType, setPolyMode, getPolyMode, NULL, " group='Display' keyincr=f ");

	TwAddVarRO(bar, "Focused Model", TW_TYPE_INT16, &camerafollow, " group='Camera' ");
	TwAddVarRW(bar, "Radius", TW_TYPE_INT16, &r, " group='Camera' min=3 ");
	TwAddVarRO(bar, "Alpha", TW_TYPE_INT16, &alpha, " group='Camera' ");
	TwAddVarRO(bar, "Beta", TW_TYPE_INT16, &beta, " group='Camera' ");

	TwStructMember pointMembers[] = {
		{ "x", TW_TYPE_FLOAT, offsetof(Point3D, x), NULL },
		{ "y", TW_TYPE_FLOAT, offsetof(Point3D, y), NULL },
		{ "z", TW_TYPE_FLOAT, offsetof(Point3D, z), NULL } };
	TwType pointType = TwDefineStruct("POINT", pointMembers, 3, sizeof(Point3D), NULL, NULL);
	TwAddVarRO(bar, "Center", pointType, &center, " group='Camera' ");


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
