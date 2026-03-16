/// MPC-MPG cviceni 6 - transformace
#include <stdlib.h>
// #include <GL\glut.h>
#include <GLUT/glut.h>
#include <string>
//#include <iostream>

using namespace std;

bool timerOn = false;

int angle = -15;

int window1, window2;

GLdouble zkos[16] = { 1.0, 0.2, 0.0, 0.0,
					  0.0, 1.2, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  0.0, 0.0, 0.0, 1.0 };

// menu
#define MENU_RESET  1001
#define MENU_EXITOK 1002
#define MENU_EXITNO 1003 
#define MENU_TIMERON 1004
#define MENU_TIMEROFF 1005

void bitmapText(int x, int y, void* font, string string)
{
	glRasterPos2f(static_cast<GLfloat>(x), static_cast<GLfloat>(y));

	int delka = string.size();

	for (int i = 0; i < delka; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

}

void vykresliDomecek()
{
	glLineWidth(1);

	glBegin(GL_LINE_STRIP);
	glVertex2i(50, 50);
	glVertex2i(150, 50);
	glVertex2i(50, 150);
	glVertex2i(150, 150);
	glVertex2i(100, 200);
	glVertex2i(50, 150);
	glVertex2i(50, 50);
	glVertex2i(150, 150);
	glVertex2i(150, 50);
	glEnd();
}

void vykresliObjekt()
{
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-5.0f, -5.0f, -5.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-5.0f, -5.0f, 5.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(5.0f, -5.0f, 5.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(5.0f, -5.0f, -5.0f);

	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-5.0f, 5.0f, -5.0f);
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-5.0f, 5.0f, 5.0f);
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(5.0f, 5.0f, 5.0f);
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(5.0f, 5.0f, -5.0f);

	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-5.0f, -5.0f, -5.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-5.0f, -5.0f, 5.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-5.0f, 5.0f, 5.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-5.0f, 5.0f, -5.0f);

	glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(5.0f, -5.0f, -5.0f);
	glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(5.0f, -5.0f, 5.0f);
	glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(5.0f, 5.0f, 5.0f);
	glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(5.0f, 5.0f, -5.0f);
	glEnd();


}

float fov = 60.0;
float nearPlane = 0.01;
float farPlane = 90.0;

void onReshape(int w, int h)             // event handler pro zmenu velikosti okna
{
	glViewport(0, 0, w, h);              // OpenGL: nastaveni rozmenu viewportu

	int window = glutGetWindow();
	if (window == window1) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);

	}
	else if (window == window2) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-double(w)/2, double(w)/2, -double(h)/2, double(h)/2, -1, 1);

	}

}

void onDisplay3D(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 4, 16,   // eye position
	         0, 0, 0,    // looking at origin
	         0, 1, 0);   // up is +Y
	vykresliObjekt();


	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
	glutPostRedisplay();
}

void onDisplay2D(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50, 50, 0);
	glScalef(0.5, 0.5, 1.0);
	glRotatef(-15, 0, 0, 1);   // clockwise = negative angle
	vykresliDomecek();

	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
	glutPostRedisplay();
}


// casovac
void onTimer(int value)
{
	if (timerOn)
	{
		// doplnte kod (ukol 3)

	}
}

// vytvoreni menu
void createMenu(void(*func)(int value))
{
	// vytvoreni podmenu
	int idSub = glutCreateMenu(func);
	glutAddMenuEntry("Ano", MENU_EXITOK);
	glutAddMenuEntry("Ne", MENU_EXITNO);

	int idTimer = glutCreateMenu(func);
	glutAddMenuEntry("Spustit", MENU_TIMERON);
	glutAddMenuEntry("Zastavit", MENU_TIMEROFF);

	// vytvoreni hlavniho menu
	glutCreateMenu(func);
	glutPostRedisplay();
	glutAddSubMenu("Casovac", idTimer);
	glutAddMenuEntry("Reset pozice ", MENU_RESET);
	glutAddSubMenu("Konec", idSub);
	glutPostRedisplay();

	// prirazeni hlavniho menu na tlacitko mysi
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(15, onTimer, 10);
}

// obsluha menu
void onMenu(int value)
{
	switch (value)
	{
	case MENU_RESET:
		angle = -15;
		break;
	case MENU_EXITOK:
		exit(1);
		break;
	case MENU_EXITNO:
		break;
	case MENU_TIMERON:
		if (!timerOn)
		{
			glutTimerFunc(15, onTimer, value);
		}
		timerOn = true;
		break;
	case MENU_TIMEROFF:
		timerOn = false;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // inicializace
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);

	window1 = glutCreateWindow("Cviceni 6 - zobrazeni window 1");
	glutDisplayFunc(onDisplay3D);	// registrace obsluhy prekresleni
	glutReshapeFunc(onReshape);		// registrace obsluhy zmeny velikosti
	createMenu(onMenu);


	glutInitWindowPosition(700, 200);

	window2 = glutCreateWindow("Cviceni 6 - transformace - window 2");
	glutDisplayFunc(onDisplay2D);	// registrace obsluhy prekresleni
	glutReshapeFunc(onReshape);		// registrace obsluhy zmeny velikosti
	createMenu(onMenu);

	glutMainLoop();
	return 0;
}
