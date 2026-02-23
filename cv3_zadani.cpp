// MPC-MPG Lab 3: Uvod do vykreslovani, Bezierove krivky
#include <GLUT/glut.h>
#include "simpleMesh.h"
#include <string>
#include <cmath>

bool trianglestrip = true;
int window1, window2, window3, winWidth;

float xOffset = 0;
float yOffset = 0;

using namespace std;

#define POCET_RIDICICH_BODU 4
int aktBod = POCET_RIDICICH_BODU;

float ridiciBody[POCET_RIDICICH_BODU][3] = {
	{60, 400, 0.0},
	{300, 300, 0.0},
	{350, 150, 0.0},
	{60, 60, 0.0}};

void zapniAA()
{
	glEnable(GL_LINE_SMOOTH);		   // Aktivace antialiasingu - funguje pouze spolecne se zapnutim pruhlednosti!!!
	glHint(GL_LINE_SMOOTH, GL_NICEST); // Nastaveni Algoritmu
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void vypniAA()
{
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
}

void vykresliKruh(int numVerticies, int sx, int sy, int r)
{
	float t = 0;

	glBegin(GL_POLYGON);
	for (int i = 0; i < numVerticies; i++)
	{
		glVertex3f(sx + r * cos(2 * 3.14 * t), sy + r * sin(2 * 3.14 * t), 0.0);
		t += 1. / numVerticies;
	}
	glEnd();
}

void opakovaniUkol1()
{

	bool antialiasing = true;

	if (antialiasing)
	{
		zapniAA();
	}

	glPointSize(25);
	glColor3f(0, 0, 1);
	glBegin(GL_POINTS);
	glVertex2f(200, 350);
	glVertex2f(300, 350);
	glEnd();

	glLineWidth(25);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(150, 300);
	glVertex2f(350, 250);
	glEnd();

	glLineWidth(5);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xAAAA);
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(50, 250);
	glVertex2f(350, 200);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(50, 150);
	glVertex2f(350, 100);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(50, 100);
	glVertex2f(350, 150);
	glEnd();

	if (antialiasing)
	{
		vypniAA();
	}
}

void vykreslovaniPole()
{
	int tempPole[2 * numVer];

	// promenne poli ulozeny v souboru simpleMesh.h
	// pripadny offset pro posun celeho pole o xOffset a yOffset
	for (int i = 0; i < numVer; i++)
	{
		tempPole[2 * i] = coords[2 * i] + xOffset;		   // x souradnice
		tempPole[2 * i + 1] = coords[2 * i + 1] + yOffset; // y souradnice
	}

	glVertexPointer(2, GL_INT, 0, tempPole);
	glColorPointer(4, GL_FLOAT, 0, colors);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	if (trianglestrip)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numVer);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, numVer);
	}
}

void bitmapText()
{
	// TODO Ukol 4
	// Vypis souradnic vsech ridicich bodu
	string str;
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		// Doplnte kod
		// Vytvoreni string do promenne str

		// Inicializace pozice

		// Vypsani for cyklem kazdeho pismena
		for (int j = 0; j < str.length(); j++)
		{
		}
	}
}

void vykresliBeziera()
{

	glColor3f(1, 1, 0); // barva
	glLineWidth(1);

	float X_0, X_1, X_2, X_3, Y_0, Y_1, Y_2, Y_3;
	X_0 = ridiciBody[0][0];
	X_1 = ridiciBody[1][0];
	X_2 = ridiciBody[2][0];
	X_3 = ridiciBody[3][0];
	Y_0 = ridiciBody[0][1];
	Y_1 = ridiciBody[1][1];
	Y_2 = ridiciBody[2][1];
	Y_3 = ridiciBody[3][1];

	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01)
	{
		float t2 = t * t;
		float t3 = t2 * t;
		float x = (-t3 + 3 * t2 - 3 * t + 1) * X_0 + (3 * t3 - 6 * t2 + 3 * t) * X_1 + (-3 * t3 + 3 * t2) * X_2 + t3 * X_3;
		float y = (-t3 + 3 * t2 - 3 * t + 1) * Y_0 + (3 * t3 - 6 * t2 + 3 * t) * Y_1 + (-3 * t3 + 3 * t2) * Y_2 + t3 * Y_3;
		glVertex2f(x, y);
	}
	glEnd();

	glPointSize(10);
	glColor3f(1, 0, 0);
	bool antialiasing = true;
	if (antialiasing)
	{
		zapniAA();
	}
	glBegin(GL_POINTS);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		glVertex2f(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xAAAA);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < POCET_RIDICICH_BODU; i++)
	{
		glVertex2f(ridiciBody[i][0], ridiciBody[i][1]);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	if (antialiasing)
	{
		vypniAA();
	}
	vykresliKruh(100, 700, 700, 150); // ukazka na  zacatek

	// TODO ukol 4, dopln kod ve funkci
	// Vypis souradnice ridicich bodu
	bitmapText();
}

void onDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	int actwin = glutGetWindow();

	if (actwin == window1)
	{
		opakovaniUkol1();
		vykreslovaniPole();
	}

	else if (actwin == window2)
	{
		vykresliBeziera();
	}

	glFlush();
}

void onResize(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, x, 0, y, -1, 1);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitWindowSize(700, 700);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_RGBA);

	winWidth = 700;

	window1 = glutCreateWindow("Podokno 1");
	glutSetWindow(window1);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);

	window2 = glutCreateWindow("Podokno 2");
	glutSetWindow(window2);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);

	glutMainLoop();

	return 0;
}
