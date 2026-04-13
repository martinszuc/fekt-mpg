// MPC-MPG cviceni 9
// kombinace texturovani a osvetleni
#include <stdlib.h>
// #include <GL/glut.h>
#include <GLUT/glut.h>
#include <iostream>
#include <string>

#define TEXTURE_WIDTH  64
#define TEXTURE_HEIGHT 64

GLfloat materialAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat materialDiffuse[] = { 0.6f, 0.3f, 0.0f, 1.0f };
GLfloat materialSpecular[] = { 0.3f, 0.8f, 0.1f, 1.0f };

GLfloat light_position[] = { 30.0f, 0.0f, 50.0f, 0.0f };

unsigned char texture[TEXTURE_HEIGHT][TEXTURE_WIDTH][4];
GLuint textura;
const char* mode[4] = { "GL_DECAL", "GL_REPLACE", "GL_MODULATE", "GL_BLEND"};

int texture_mode = 0;


void InitTexture(void) // inicializace (vykresleni) textury sachovnice pres bitove operace
{
	int i, j, c;
	unsigned char * P;

	for (j = 0; j < TEXTURE_HEIGHT; j++) {
		P = texture[j][0];
		for (i = 0; i < TEXTURE_WIDTH; i++) {
			c = ((((i & 0x04) == 0) ^ (((j & 0x04)) == 0))) ? 255 : 0;
			*P++ = (unsigned char)c;
			*P++ = (unsigned char)(i << 2);
			*P++ = (unsigned char)(j << 2);
			*P++ = (unsigned char)200; // alfa kanal (pruhlednost) 
		}
	}
}

void bitmapText()
{
	// Vypis souradnic vsech ridicich bodu
	std::string str;
	glRasterPos2i(50, 350);
	str = mode[texture_mode];
	for (int j = 0; j < str.length(); j++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	}
}


void onInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	InitTexture();   // vytvoreni textury v bufferu texture[]

	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // zde se nastavi parametry textury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,    // zde dojde k presunu textury do pameti graficke karty
		TEXTURE_WIDTH, TEXTURE_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // perspektivni korekce zobrazeni

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


}

void onReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, 0, 50);
}

void onDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	bitmapText();
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	
	float color[4] = { 0.5, 0.5, 0.5, 0.5 };
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
	// GL_BLEND, GL_MODULATE, GL_DECAL, GL_REPLACE
	if (texture_mode == 0) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	if (texture_mode == 1) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (texture_mode == 2) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (texture_mode == 3) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glEnable(GL_TEXTURE_2D); // zapnuti 2D texturovaci jednotky


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(100, 100);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(300, 100);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(300, 300);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(100, 300);
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
	if (key >= 'A' && key <= 'Z')
		key += 'a' - 'A';

	switch (key) {
	case 27:    exit(0);            break;
	case 'q':   exit(0);            break;
	case 'a':	texture_mode = 0;	break;
	case 's':	texture_mode = 1;	break;
	case 'd':	texture_mode = 2;	break;
	case 'f':	texture_mode = 3;	break;
	default:                        break;
	}

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(30, 30);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Cviceni 9 - Uloha 5 (Textury a osvetleni)");
	std::cout << "Ovladani \n a: GL_DECAL\n s: GL_REPLACE\n d: GL_MODULATE\n f: GL_BLEND";
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	onInit();
	glutMainLoop();
	return 0;
}
