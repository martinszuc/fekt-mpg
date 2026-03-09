// MPC-MPG cviceni 5
// Subdivision bezierove bikubiky, rozdil mezi subdivision a openGL evaluatorem

#include <cmath>
#include <iostream>
#include <GLUT/glut.h> // macOS
//#include <GL\glut.h> // todo Win

using namespace std;

#define PI 3.14159265

// globalni promenne pro transformace
double xnew = 0, ynew = 0, znew = 0;	// aktualni poloha mysi
double xold = 0, yold = 0, zold = 0;	// predchozi poloha mysi
double tranznew = -100;					// aktualni posunuti ve smeru osy z
double tranzold = -50;					// predchozi posunuti ve smeru osy z
bool tocime = false;					// ovladac rotace
bool posouvame = false;					// ovladac translace
double newnorm, oldnorm;				// normy polohovych vektoru mysi
double xaxis = 0, yaxis = 0, zaxis = 0; // osa rotace // PR: dava smysl nulovy vektor?
double xaxis2, yaxis2, zaxis2;			// pomocna osa rotace
double angle = 0;						// uhel rotace
double zz;								// pomocna promenna pro vypocet posunuti ve smeru osy z
int hustota = 10;						// hustota bodu generovanych ridicimi body

// matice, do ktere se bude ukladat rotace (inicalizovava jako identita)
GLfloat rotationmatrix[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1};

// nastaveni projekce
float fov = 90.0;
float nearPlane = 0.1;
float farPlane = 500.0;

// globalni promenne pro zpetnou projekci bodu
double xw, yw, zw;
GLdouble zWindow;

float pole_boduN[7][7][3];

float ridiciBody[4][4][3] = {
	{{-15, -15, 20}, {-5, -15, 40}, {5, -15, 40}, {15, -15, 20}},
	{{-15, -5, 10}, {-5, -5, 30}, {5, -5, 30}, {15, -5, 10}},
	{{-15, 5, 0}, {-5, 5, 20}, {5, 5, 20}, {15, 5, 0}},
	{{-15, 15, -20}, {-5, 15, 0}, {5, 15, 0}, {15, 15, -20}}};

GLfloat colorPoints[2][2][4] = { // ridici body Bezierovy plochy v RGBA // PR: z komentare nechapu
	{{1.0, 1.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}},
	{{1.0, 0.0, 0.0, 0.0}, {1.0, 1.0, 1.0, 0.0}}};

// funkce volana pri zmene velikosti sceny
void onReshape(int w, int h)
{
	glViewport(0, 0, w, h);		 // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION); // OpenGL: matice bude typu projekce
	glLoadIdentity();			 // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
}

// funkce pro inicializaci OpenGL
void onInit(void)
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // perspektivni korekce zobrazeni
}

void vykresliRidiciBody()
{
	// vykresli ridici body bikubiky
	glPointSize(4.0);
	glColor3f(0.5, 0.5, 0.4);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glVertex3fv(ridiciBody[i][j]);
		}
	}
	glEnd();
}

void vykresliBezieruvPlat(GLfloat points[][4][3], int density = 20, int dimensions = 4)
// Vykresleni pole bodu pomoci openGL evaluatoru
// density: urcuje pocet bodu medzi ctverici bodu
{

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_COLOR_4);

	// layout float[u][v][xyz]: ustride = dimensions*3 (step over one u-row),
	// vstride = 3 (step to next v-point within same u-row)
	glMap2f(GL_MAP2_VERTEX_3,
		0.0f, 1.0f,        // u range
		dimensions * 3,    // ustride
		dimensions,        // uorder
		0.0f, 1.0f,        // v range
		3,                 // vstride
		dimensions,        // vorder
		&points[0][0][0]);

	glPointSize(2.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i <= density; i++)
	{
		for (int j = 0; j <= density; j++)
		{
			glEvalCoord2f((float)i / density, (float)j / density);
		}
	}
	glEnd();

	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_COLOR_4);
}

void subdivisionCasteljau()
{
	double t = 0.5; // Subdivision ratio

	// First pass: Subdivision along columns
	for (int u = 0; u < 4; u++)
	{
		for (int k = 0; k < 3; k++)
		{

			float temp = (1.0 - t) * ridiciBody[u][1][k] + t * ridiciBody[u][2][k];

			float nulty = ridiciBody[u][0][k];
			float prvni = (1.0 - t) * nulty + t * ridiciBody[u][1][k];
			float druhy = (1.0 - t) * prvni + t * temp;

			float sesty = ridiciBody[u][3][k];
			float paty = (1.0 - t) * ridiciBody[u][2][k] + t * sesty;
			float ctvrty = (1.0 - t) * temp + t * paty;

			float treti = (1.0 - t) * druhy + t * ctvrty;

			pole_boduN[u * 2][0][k] = nulty;
			pole_boduN[u * 2][1][k] = prvni;
			pole_boduN[u * 2][2][k] = druhy;
			pole_boduN[u * 2][3][k] = treti;
			pole_boduN[u * 2][4][k] = ctvrty;
			pole_boduN[u * 2][5][k] = paty;
			pole_boduN[u * 2][6][k] = sesty;
		}
	}
	for (int s = 0; s < 7; s++)
	{
		for (int k = 0; k < 3; k++)
		{
			float temp = (1.0 - t) * pole_boduN[2][s][k] + t * pole_boduN[4][s][k];

			float nulty = pole_boduN[0][s][k];
			float prvni = (1.0 - t) * nulty + t * pole_boduN[2][s][k];
			float druhy = (1.0 - t) * prvni + t * temp;

			float sesty = pole_boduN[6][s][k];
			float paty = (1.0 - t) * pole_boduN[4][s][k] + t * pole_boduN[6][s][k];
			float ctvrty = (1.0 - t) * temp + t * paty;

			float treti = (1.0 - t) * druhy + t * ctvrty;

			pole_boduN[0][s][k] = nulty;
			pole_boduN[1][s][k] = prvni;
			pole_boduN[2][s][k] = druhy;
			pole_boduN[3][s][k] = treti;
			pole_boduN[4][s][k] = ctvrty;
			pole_boduN[5][s][k] = paty;
			pole_boduN[6][s][k] = sesty;
		}
	}
}

void zobrazMesh()
{
	// Funkce pro zobrazeni meshe vznikle pomoci subdivision a bodu
	glPointSize(4.0);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	int rows = 7;
	glBegin(GL_POINTS);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			glVertex3fv(pole_boduN[i][j]);
		}
	}
	glEnd();

	glColor4f(1.0, 1.0, 0.0, 0.1);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			glVertex3fv(pole_boduN[i][j]);		   // Bottom-left
			glVertex3fv(pole_boduN[i + 1][j]);	   // Bottom-right
			glVertex3fv(pole_boduN[i + 1][j + 1]); // Top-right
			glVertex3fv(pole_boduN[i][j + 1]);	   // Top-left
		}
	}
	glEnd();
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// translace
	glTranslatef(0, 0, tranznew);

	// rotace
	glMultMatrixf(rotationmatrix);

	// povoleni 2-D evaluatoru
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	subdivisionCasteljau();
	vykresliRidiciBody();
	zobrazMesh();

	glDisable(GL_BLEND);

	vykresliBezieruvPlat(ridiciBody, 50);

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

void onKeyHandler(unsigned char key, int x, int y)
{

	if (key == '0')
		hustota = pow(2, 2);
	if (key == '1')
		hustota = pow(2, 3);
	if (key == '2')
		hustota = pow(2, 4);
	if (key == '3')
		hustota = pow(2, 5);
	if (key == '4')
		hustota = pow(2, 6);

	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);									   // inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // init double buffer

	glutInitWindowSize(640, 480);	  // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(100, 200); // nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("B�zier�v pl�t"); // vytvoreni okna
	glutDisplayFunc(onDisplay);		   // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);		   // registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	glutKeyboardFunc(onKeyHandler);

	onInit();
	glutMainLoop(); // nekonecna smycka, interakce uz jen pomoci event handleru

	return 0; // ukonceni programu
}
