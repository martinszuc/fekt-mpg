// MPC-MPG cviceni 10 - Blender
#include "OBJ_Loader.h"
#include "imageLoad.h"
#include <GLUT/glut.h>
#define PI 3.141592653589793f

objl::Loader Objekt;
unsigned int textura;
bool loaded;

// globalni promenne pro transformace
double xnew = 0, ynew = 0, znew = 0; // aktualni poloha mysi
double xold = 0, yold = 0, zold = 0; // predchozi poloha mysi
double tranznew = -50; // aktualni posunuti ve smeru osy z
double tranzold = -50; // predchozi posunuti ve smeru osy z
bool tocime = false; // ovladac rotace
bool posouvame = false; // ovladac translace
double newnorm, oldnorm; // normy polohovych vektoru mysi
double xaxis = 0, yaxis = 0, zaxis = 0; // osa rotace
double xaxis2, yaxis2, zaxis2; // pomocna osa rotace
double angle = 0; // uhel rotace
double zz; // pomocna promenna pro vypocet posunuti ve smeru osy z

// matice, do ktere se bude ukladat rotace (inicalizovava jako identita)
GLfloat rotationmatrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

float fov = 80.0;
float nearPlane = 0.1;
float farPlane = 100;

// vlastnosti svetel a materialu
GLfloat lightPosition[] = { 2.0f,  3.0f,  1.0f,  0.0f };
GLfloat lightAmbient[] = { 1.0f,  1.0f,  1.0f,  1.0f };
GLfloat lightDiffuse[] = { 1.0f,  1.0f,  0.0f,  1.0f };
GLfloat lightSpecular[] = { 1.0f,  0.0f,  1.0f,  1.0f };

GLfloat materialAmbient[] = { 0.7f,  0.7f,  0.7f,  1.0f };
GLfloat materialDiffuse[] = { 0.5f,  0.5f,  0.5f,  1.0f };
GLfloat materialSpecular[] = { 1.0f,  0.0f,  0.0f,  1.0f };
GLfloat materialShininess = 80.0f;

void onInit()
{
	// Doplnte: nacteni objektu, promenna loaded indikuje, zda se nacteni povedlo
	loaded = Objekt.LoadFile("tyre.obj");

	// Doplnte: nacteni textury
	setTexture("tyre.bmp", &textura, true);
}

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

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// nastaveni sten
	glShadeModel(GL_SMOOTH);

	// Doplnte: nastaveni svetla
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	// translace
	glTranslatef(0, 0, tranznew);

	// rotace
	glMultMatrixf(rotationmatrix);

	// Doplnte: nastaveni vlastnosti materialu
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// Doplnte: aktivovani textury a nastaveni prostredi
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	if (loaded)
	{
		// Doplnte: vytvoreni vertexu pomoci vsech nactenych meshu, pouziti spravne primitivy
		for (int i = 0; i < Objekt.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Objekt.LoadedMeshes[i];
			int meshsize = curMesh.Vertices.size();
			// Ukol 2
			// Pomoci cyklu doplnte vykresleni objektu s spravnym texturovanim a osvetlenim
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < meshsize; j++) {
				objl::Vertex v = curMesh.Vertices[j];
				glNormal3f(v.Normal.X, v.Normal.Y, v.Normal.Z);
				glTexCoord2f(v.TextureCoordinate.X, v.TextureCoordinate.Y);
				glVertex3f(v.Position.X, v.Position.Y, v.Position.Z);
			}
			glEnd();

		}
	}

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

	glutInitWindowSize(600, 600);       // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 100);   // nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("Cviceni 10 - Ukol 2 (Blender)");		// vytvoreni okna
	onInit();
	glutDisplayFunc(onDisplay);         // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);         // registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMotion);
	glutMainLoop();                     // nekonecna smycka, interakce uz jen pomoci event handleru

	return 0;                           // ukonceni programu
}
