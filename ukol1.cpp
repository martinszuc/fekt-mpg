// MPC-MPG cviceni 10 - kvadriky
#include <stdlib.h>
#include <GL/glut.h>
#include "imageLoad.h"

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64
#define PI 3.141592653589

// globalni promenne pro transformace
double xnew = 0, ynew = 0, znew = 0;	// aktualni poloha mysi
double xold = 0, yold = 0, zold = 0;	// predchozi poloha mysi
double tranznew = -50;					// aktualni posunuti ve smeru osy z
double tranzold = -50;					// predchozi posunuti ve smeru osy z
bool tocime = false;					// ovladac rotace
bool posouvame = false;					// ovladac translace
double newnorm, oldnorm;				// normy polohovych vektoru mysi
double xaxis = 0, yaxis = 0, zaxis = 0; // osa rotace
double xaxis2, yaxis2, zaxis2;			// pomocna osa rotace
double angle = 0;						// uhel rotace
double zz;								// pomocna promenna pro vypocet posunuti ve smeru osy z

// matice, do ktere se bude ukladat rotace (inicalizovana jako identita)
GLfloat rotationmatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

bool mipmap = false;

// materialy
GLfloat materialAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat materialDiffuse[] = {0.8f, 0.2f, 0.2f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShininess = 80;

// osvetleni
GLfloat lightPosition[] = {-30.0f, 50.0f, 50.0f, 1.0f};
GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightDiffuse[] = {0.8f, 0.2f, 0.2f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

unsigned char texture[TEXTURE_HEIGHT][TEXTURE_WIDTH][4];
GLuint textury[2];

void InitTexture(void)
{
	int i, j, c;
	unsigned char *P;

	for (j = 0; j < TEXTURE_HEIGHT; j++)
	{
		P = texture[j][0];
		for (i = 0; i < TEXTURE_WIDTH; i++)
		{
			c = ((((i & 0x02) == 0) ^ (((j & 0x02)) == 0))) ? 255 : 0;
			*P++ = (unsigned char)c;
			*P++ = (unsigned char)c;
			*P++ = (unsigned char)c;
			*P++ = (unsigned char)255;
		}
	}
}

void onInit(void)
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	glDisable(GL_CULL_FACE);

	// Ukol 1 - doplnte nastaveni materialu
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// Ukol 1 - doplnte nastaveni svetla
	// nastaveni hodnot ambient, diffuse, specular a position na prednastavene hodnoty pro svetlo c. 0 (GL_LIGHT0)
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	// Ukol 1
	// zapnuti svetla c. 0 a zapnuti osvetleni (GL_LIGHTIING)
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	InitTexture(); // vytvoreni textury sachovnice v bufferu texture[]

	glGenTextures(1, &textury[0]);
	glBindTexture(GL_TEXTURE_2D, textury[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, // zde dojde k presunu textury do pameti graficke karty
				 TEXTURE_WIDTH, TEXTURE_HEIGHT,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	// Ukol 1 - doplnte nacteni textury z .bmp obrazku do adresy &textury[1]

	setTexture("textura.bmp", &textury[1], mipmap);
	glBindTexture(GL_TEXTURE_2D, textury[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // perspektivni korekce zobrazeni

	glEnable(GL_TEXTURE_2D);
}

void onReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)w / h, 0.1, 200.0);
}

void onDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// translace
	glTranslatef(0, 0, tranznew);

	// rotace
	glMultMatrixf(rotationmatrix);

	GLUquadric *quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GLU_TRUE);

	// Ukol 1 - vytvorte kouli, nastavte barvu na cervenou a aplikujte translaci
	glPushMatrix();
	glTranslatef(-15.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluSphere(quadric, 5.0, 32, 32);
	glPopMatrix();

	// Ukol 1 pokracovani - vytvorte objekty, doplnte aplikace textur a nastaveni interakce textury s prostredim (glTexEnvi)
	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, textury[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluDisk(quadric, 5.0, 7.0, 32, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluCylinder(quadric, 6.0, 6.0, 10.0, 32, 8);
	glPopMatrix();

	gluDeleteQuadric(quadric);

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
	glutInitWindowSize(600, 400);
	glutCreateWindow("Cviceni 10 - Ukol 1 (Kvadriky)");
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	onInit();
	glutMainLoop();
	return 0;
}
