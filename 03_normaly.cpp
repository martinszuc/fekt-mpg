// MPC-MPG cviceni 8 - programovy vypocet normal
#include "stdlib.h"
#include "math.h"
#include <GL/glut.h>

#define POLOMER 20
#define PI		3.141592653589793f
#define SIRKA	5

// globalni promenne pro transformace
double xnew = 0, ynew = 0, znew = 0; // aktualni poloha mysi
double xold = 0, yold = 0, zold = 0; // predchozi poloha mysi
double tranznew = 0; // aktualni posunuti ve smeru osy z
double tranzold = 0; // predchozi posunuti ve smeru osy z
bool tocime = false; // ovladac rotace
bool posouvame = false; // ovladac translace
double newnorm, oldnorm; // normy polohovych vektoru mysi
double xaxis = 0, yaxis = 0, zaxis = 0; // osa rotace
double xaxis2, yaxis2, zaxis2; // pomocna osa rotace
double angle = 0; // uhel rotace
double zz; // pomocna promenna pro vypocet posunuti ve smeru osy z

// matice, do ktere se bude ukladat rotace (inicalizovava jako identita)
GLfloat rotationmatrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

// nastaveni projekce
float fov = 60.0;
float nearPlane = 0.1;
float farPlane = 390.0;

// vlastnosti svetel a materialu
GLfloat lightPosition[] = { 0.0f, 0.0f, 20.0f, 0.0f };
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// funkce pro vypocet normaly
void vypoctiNormalu(float vert1[], float vert2[], float vert3[], float *nx, float *ny, float *nz)
{
	// vektor od vert1 do vert2
	float v12x, v12y, v12z;

	// vektor od vert1 do vert3
	float v13x, v13y, v13z;
	float invnorm;

	// výpoèet vektorù
	v12x = vert2[0] - vert1[0];
	v12y = vert2[1] - vert1[1];
	v12z = vert2[2] - vert1[2];
	v13x = vert3[0] - vert1[0];
	v13y = vert3[1] - vert1[1];
	v13z = vert3[2] - vert1[2];

	// vektorový souèin v12 x v13
	*nx = v12y * v13z - v12z * v13y;
	*ny = v12z * v13x - v12x * v13z;
	*nz = v12x * v13y - v12y * v13x;

	// 1 / norma vektorového souèinu
	invnorm = 1.0f / sqrt((*nx)*(*nx) + (*ny)*(*ny) + (*nz)*(*nz));

	// normování
	*nx *= invnorm;
	*ny *= invnorm;
	*nz *= invnorm;
}

// vykresleni objektu
void vykresli(int ploch) {

	float uhel = 2.0f*PI / ploch;
	float v1[3], v2[3], v3[3], v4[3];
	float n[3];

	glPushMatrix();

	// translace
	glTranslatef(0, 0, tranznew);

	// rotace
	glMultMatrixf(rotationmatrix);

	glBegin(GL_QUADS);
	for (int i = 0; i < ploch; i++) {
		v1[0] = cos(uhel*i)*POLOMER;
		v1[1] = sin(uhel*i)*POLOMER;
		v1[2] = SIRKA;

		v2[0] = cos(uhel*(i + 1))*POLOMER;
		v2[1] = sin(uhel*(i + 1))*POLOMER;
		v2[2] = SIRKA;

		v3[0] = v2[0];
		v3[1] = v2[1];
		v3[2] = -SIRKA;

		v4[0] = v1[0];
		v4[1] = v1[1];
		v4[2] = -SIRKA;

		// ukol 4 - doplnte kod a definice normal

		// vypoctiNormalu(...



		// glNormal3fv(....



		// glVertex3fv(....



	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < ploch; i++) {
		v1[0] = cos(-uhel * i)*POLOMER;
		v1[1] = sin(-uhel * i)*POLOMER;
		v1[2] = SIRKA;

		// ukol 4 - doplnte kod



	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < ploch; i++) {
		v1[0] = cos(uhel*i)*POLOMER;
		v1[1] = sin(uhel*i)*POLOMER;
		v1[2] = -SIRKA;

		// ukol 4 - doplnte kod


	}
	glEnd();

	glPopMatrix();
}

// funkce volana pri zmene velikosti sceny
void onReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
}

// funkce volana pri prekresleni sceny
void onDisplay(void)
{
	// nastaveni parametru mazani
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);

	// mazani rovin
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// steny
	glFrontFace(GL_CW);					// nastaveni front == clockwise
	glPolygonMode(GL_FRONT, GL_FILL);   // mod predni steny fill
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// nastaveni sten
	glShadeModel(GL_SMOOTH);            // smooth shading

	// nastaveni svetla
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	// zapnuti osvetleni
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// nastaveni kamery
	gluLookAt(4.0f, 22.0f, 48.0f, // bod, odkud se kamera diva
		0.0f, 0.0f, 0.0f,         // bod, kam se kamera diva
		0.0f, 1.0f, 0.0f);        // poloha "stropu" ve scene

	glColor3f(0.6f, 0.2f, 0.1f);
	glEnable(GL_COLOR_MATERIAL);
	vykresli(10);

	// provedeni a vykresleni zmen
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
		else {
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

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);              // inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(400, 400);       // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 200);   // nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("MPC-MPG cviceni 8 - ukol 4");		// vytvoreni okna
	glutDisplayFunc(onDisplay);         // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);          // registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	glutMainLoop();                     // nekonecna smycka, interakce uz jen pomoci event handleru

	return 0;                           // ukonceni programu
}