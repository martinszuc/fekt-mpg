// MPC-MPG cviceni 8 - osvetleni sceny
#include <GL/glut.h>
#include "math.h"

#define PI		3.141592653589793f

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

// nastaveni perspektivni projekce
float fov = 60.0;
float nearPlane = 0.1;
float farPlane = 90.0;

// vlastnosti svetel a materialu
GLfloat lightPosition[] = { .0f, .0f, 10.0f, .0f };
GLfloat lightAmbient[] = { .2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightSpecular[] = { .0f, .0f, .0f, 1.0f };
GLfloat lightDirection[] = { .0f, .0f, -1.0f };

GLfloat materialAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat materialSpecular[] = { .0f, .0f, .0f, 1.0f };
GLfloat materialShininess = 30.0f;

void onReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
}

void onDisplay(void)
{
	// nastaveni parametru mazani
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);

	// mazani rovin
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// steny
	glFrontFace(GL_CW);					// nastaveni front == clockwise
	glPolygonMode(GL_FRONT, GL_FILL);   // mod predni steny fill
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Ukol 3

	// nastaveni sten (shading, glShadeModel)


	// zapnuti svetla 0


	// nastaveni GL_AMBIENT, GL_DIFFUSE a GL_SPECULAR


	// nastaveni reflektoroveho svetla s GL_SPOT_CUTOFF = 180 a GL_SPOT_DIRECTION = lightDirection


	// nastaveni materialu


	// nastaveni kamery
	gluLookAt(4.0f, 22.0f, 48.0f,       // bod, odkud se kamera diva
		0.0f, 0.0f, 0.0f,               // bod, kam se kamera diva
		0.0f, 1.0f, 0.0f);              // smer nahoru ve scene

	// translace
	glTranslatef(0, 0, tranznew);

	// rotace
	glMultMatrixf(rotationmatrix);

	// krychle - vytvorte symetricky okolo pocatku, delka hrany 20
	//         - pouze front faces; FRONT = definovana po smeru hodinovych rucicek
	//         - zadefinujte korektni normaly
	//
	//         +y
	//         |
	//         |
	//         /------- +x
	//        /
	//       /
	//      +z

	glBegin(GL_QUADS);

	// Ukol 3 - doplnte krychli o strane 20 s normalou kolmou vzdy ke stene

	// predni strana


	// zadni strana


	// leva strana


	// prava strana


	// horni strana


	// dolni strana


	glEnd();

	// provedeni a vykresleni zmen
	glFlush();
	glutSwapBuffers();
}

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // init double buffer

	glutInitWindowSize(400, 400);       // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 200);   // nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("MPC-MPG cviceni 8 - ukol 3");	// vytvoreni okna
	glutDisplayFunc(onDisplay);         // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);         // registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	glutMainLoop();                     // nekonecna smycka, interakce uz jen pomoci event handleru

	return 0;                           // ukonceni programu
}