// MPC-MPG cviceni 9
// textura - nahrani vlastni textury, korektni texturovani, parametry
#include <stdlib.h>
#include "imageLoad.h"
// #include <GL/glut.h>
#include <GLUT/glut.h>
#include "math.h"

#define PI 3.141592653589793f

// globalni promenne pro transformace
double xnew = 0, ynew = 0, znew = 0;	// aktualni poloha mysi
double xold = 0, yold = 0, zold = 0;	// predchozi poloha mysi
double tranznew = 0;					// aktualni posunuti ve smeru osy z
double tranzold = 0;					// predchozi posunuti ve smeru osy z
bool tocime = false;					// ovladac rotace
bool posouvame = false;					// ovladac translace
double newnorm, oldnorm;				// normy polohovych vektoru mysi
double xaxis = 0, yaxis = 0, zaxis = 0; // osa rotace
double xaxis2, yaxis2, zaxis2;			// pomocna osa rotace
double angle = 0;						// uhel rotace
double zz;								// pomocna promenna pro vypocet posunuti ve smeru osy z

// matice, do ktere se bude ukladat rotace (inicalizovava jako identita)
GLfloat rotationmatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

// rozmery okna
int windowWidth, windowHeight;

// pole pro nazvy textur
GLuint textury[1];

bool mipmap = false;

void onInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	if (!setTexture("textura.bmp", &textury[0], mipmap))
	{
		std::cout << "Chyba pri nacitani souboru" << std::endl;
		exit((int)0);
	}

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (mipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
}

void onReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
}

void setPerspectiveProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (double)windowWidth / (double)windowHeight, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void onDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setPerspectiveProjection();

	// nastaveni kamery
	gluLookAt(0.0f, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// translace
	glTranslatef(0, 0, tranznew / 10);

	// rotace
	glMultMatrixf(rotationmatrix);

	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Ukol 1 - doplnte kod

	// Ukol 2 - doplnte kod

	glFlush();
	glutSwapBuffers();
}

// funkce volana pri stisku tlacitka
void onMouseButton(int button, int state, int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	// po stisku leveho tlacitka
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			tocime = true;

			// zmena rozsahu na -1 az 1
			xold = x / ((double)glutGet(GLUT_WINDOW_WIDTH) / 2) - 1;
			yold = y / ((double)glutGet(GLUT_WINDOW_HEIGHT) / 2) - 1;

			// dopocet hodnoty z
			float z = 1 - xold * xold - yold * yold;
			zold = z > 0 ? sqrt(z) : 0;

			// normalizace
			oldnorm = sqrt(xold * xold + yold * yold + zold * zold);
			xold /= oldnorm;
			yold /= oldnorm;
			zold /= oldnorm;
		}
		else
		{
			tocime = false;
		}
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			posouvame = true;
			zz = y;
		}
		else
		{
			posouvame = false;
			tranzold = tranznew;
		}
	}
	glutPostRedisplay();
}

// funkce volana pri pohybu mysi a stisknutem tlacitku mysi
void onMouseMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	if (tocime)
	{
		// zmena rozsahu na -1 az 1
		xnew = x / ((double)glutGet(GLUT_WINDOW_WIDTH) / 2) - 1;
		ynew = y / ((double)glutGet(GLUT_WINDOW_HEIGHT) / 2) - 1;

		// dopocet hodnoty z
		float z = 1 - xnew * xnew - ynew * ynew;
		znew = z > 0 ? sqrt(z) : 0;

		// normalizace
		newnorm = sqrt(xnew * xnew + ynew * ynew + znew * znew);
		xnew /= newnorm;
		ynew /= newnorm;
		znew /= newnorm;

		// vypocet aktualni osy rotace
		xaxis = yold * znew - ynew * zold;
		yaxis = -1 * (xold * znew - xnew * zold);
		zaxis = xold * ynew - xnew * yold;

		// vypocet uhlu rotace
		angle = 0;
		if (xnew * xold + ynew * yold + znew * zold < 1)
			angle = 180 * acos(xnew * xold + ynew * yold + znew * zold) / PI;

		// ulozeni pozice mysi pro pouziti v pristim volani funkce
		xold = xnew;
		yold = ynew;
		zold = znew;

		// rotatece osy tak, aby byly anulovany dosavadni rotace (protoze rotujeme vzhledem ke stastickemu systemu)
		xaxis2 = rotationmatrix[0] * xaxis + rotationmatrix[1] * yaxis + rotationmatrix[2] * zaxis;
		yaxis2 = rotationmatrix[4] * xaxis + rotationmatrix[5] * yaxis + rotationmatrix[6] * zaxis;
		zaxis2 = rotationmatrix[8] * xaxis + rotationmatrix[9] * yaxis + rotationmatrix[10] * zaxis;
		xaxis = xaxis2;
		yaxis = yaxis2;
		zaxis = zaxis2;

		// ulozeni rotace do matice
		glPushMatrix();
		glLoadMatrixf(rotationmatrix);
		glRotatef(angle, xaxis, yaxis, zaxis);
		glGetFloatv(GL_MODELVIEW_MATRIX, rotationmatrix);
		glPopMatrix();

		glutPostRedisplay();
	}
	if (posouvame)
	{
		tranznew = tranzold + y - zz;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(30, 30);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cviceni 9 - Ukol 1, 2 (Textury)");
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	onInit();
	glutMainLoop();
	return 0;
}
