#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

int rotate = 0;

int scaleY = 1;

int moveX = 0;
int moveY = 0;
int moveZ = 0;


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
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	

// put the geometric transformations here
	glTranslatef(moveX, moveY, moveZ);
	glRotatef(rotate, 0, 1, 0); 
	glScalef(1, scaleY, 1);
	

// put drawing instructions here
	glBegin(GL_TRIANGLES);
		

		//base
		glColor3f(0, 1, 1);

		glVertex3f(-1, 0, -1);
		glVertex3f(1, 0, -1);
		glVertex3f(1, 0, 1);

		glVertex3f(1, 0, 1);
		glVertex3f(-1, 0, 1);
		glVertex3f(-1, 0, -1);

		//face 1
		glColor3f(0, 1, 0);
		glVertex3f(1, 0, -1);
		glVertex3f(0, 1, 0);
		glVertex3f(1, 0, 1);

		//face 2
		glColor3f(0, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(-1, 0, 1);

		//face 3
		glColor3f(1, 0, 0);
		glVertex3f(-1, 0, -1);
		glVertex3f(0, 1, 0);
		glVertex3f(1, 0, -1);

		//face 4
		glColor3f(1, 1	, 0);
		glVertex3f(-1, 0, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(-1, 0, -1);

	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events


void keyboard_func(unsigned char key, int x, int y) {
	switch (key)
	{
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
	case '4':
		glCullFace(GL_FRONT);
		renderScene();
		break;
	case '5':
		glCullFace(GL_BACK);
		renderScene();
		break;
	case 'w':
		moveZ += 1;
		renderScene();
		break;
	case 's':
		moveZ -= 1;
		renderScene();
		break;
	case 'a':
		moveX -= 1;
		renderScene();
		break;
	case 'd':
		moveX += 1;
		renderScene();
		break;
	case 'q':
		moveY += 1;
		renderScene();
		break;
	case 'e':
		moveY -= 1;
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
		scaleY += 1;
		renderScene();
		break;
	case GLUT_KEY_DOWN:
		scaleY -= 1;
		renderScene();
		break;
	case GLUT_KEY_LEFT:
		rotate += 10;
		renderScene();
		break;
	case GLUT_KEY_RIGHT:
		rotate -= 10;
		renderScene();
		break;
	default:
		break;
	}
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
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
