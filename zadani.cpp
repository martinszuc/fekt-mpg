#include <GL/glew.h>
#include <GLUT/glut.h>
#include "imageLoad.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

float angle = 0.0;
int width, height;
float aspectRatio;
bool timerOn = false;
GLfloat mvp[16];

#define MENU_EXITOK 1002
#define MENU_EXITNO 1003 
#define MENU_TIMERON 1004
#define MENU_TIMEROFF 1005

GLuint points_vbo, colors_vbo, uv_vbo, vertex_shader, fragment_shader, program;
GLuint mvp_location, vp_location, texture0_location, texture1_location;
GLuint textury[2];

// souradnice bodu
float points[] = {
   0.700,  0.000,
  -0.350,  0.606,
  -0.350, -0.606
};

// barvy bodu
float colors[] = {
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0
};

// texturovaci souradnice
float uv[] = {
  1.000, 0.500,
  0.250, 0.933,
  0.250, 0.067
};

static const char* vertex_shader_text =
"#version 460\n"
"uniform mat4 MVP;"
"layout(location = 0) in vec2 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"layout(location = 2) in vec2 vertex_uv;"
"out vec3 color;"
"out vec2 uv;"
"void main()"
"{"
"    gl_Position = MVP * vec4(vertex_position, 0.0, 1.0);"
"    color = vertex_color;"
"    uv = vertex_uv;"
"}";

static const char* fragment_shader_text =
"#version 460\n"
"uniform vec2 viewportDimensions;"
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"in vec3 color;"
"in vec2 uv;"
"out vec4 FragColor;"
"void main()"
"{"
"    FragColor = vec4(color, 1.0);"
"}";

void onReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	aspectRatio = width / (float)height;
}

void onInit()
{
	std::cout << glGetString(GL_VERSION) << "\n";
	glewInit();

	// nacteni textury
	if (!setTexture("bricks.bmp", &textury[0], true)) {
		std::cout << "Chyba pri nacitani souboru." << std::endl;
		exit((int)0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (!setTexture("grass.bmp", &textury[1], true)) {
		std::cout << "Chyba pri nacitani souboru." << std::endl;
		exit((int)0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// vytvoreni VBO pro souradnice vertexu
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// vytvoreni VBO pro barvy vertexu
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	// vytvoreni VBO pro texturovaci souradnice vertexu
	glGenBuffers(1, &uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

	// navazani na vertex array object
	// pozice 0 - points
	// pozice 1 - colors
	// pozice 2 - uv
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// zapnuti atributu
	// pozice 0 - points
	// pozice 1 - colors
	// pozice 2 - uv
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// vytvoreni vertex shaderu
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	// vytvoreni fragment shaderu
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	// svazani obou shaderu do programu
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// ulozeni umisteni uniform matice MVP, abychom ji dale mohli menit
	mvp_location = glGetUniformLocation(program, "MVP");
	vp_location = glGetUniformLocation(program, "viewportDimensions");

	// ulozeni umisteni textur, abychom jich mohli pouzivat vic naraz
	texture0_location = glGetUniformLocation(program, "texture0");
	texture1_location = glGetUniformLocation(program, "texture1");
}

void onDisplay(void)
{
	// nastaveni parametru mazani
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// prepocet rotace podle casovace
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle, 0.0, 0.0, 1.0);

	// prepocet ortogonalni projekce
	glOrtho(-aspectRatio, aspectRatio, -1, 1, -1, 1);

	// ulozeni transformaci do matice mvp, aby je slo predat do vertex shaderu
	glGetFloatv(GL_MODELVIEW_MATRIX, mvp);

	// aplikovani obou shaderu (vertex, fragment) svazanych v programu
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
	glUniform2f(vp_location, (float)width, (float)height);
	glUniform1i(texture0_location, 0);
	glUniform1i(texture1_location, 1);

	// propojeni textur
	glActiveTexture(GL_TEXTURE0 + 0); // texturovaci jednotka 0
	glBindTexture(GL_TEXTURE_2D, textury[0]);
	glActiveTexture(GL_TEXTURE0 + 1); // texturovaci jednotka 1
	glBindTexture(GL_TEXTURE_2D, textury[1]);

	// vykresleni pole vertexu
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// provedeni a vykresleni zmen
	glFlush();
	glutSwapBuffers();
}

void onTimer(int value)
{
	angle += 1;
	glutPostRedisplay();
	if (timerOn)
	{
		glutTimerFunc(10, onTimer, value);
	}
}

void onMenu(int value)
{
	switch (value)
	{
	case MENU_EXITOK: exit(1); break;
	case MENU_EXITNO: break;
	case MENU_TIMERON:
		if (!timerOn)
		{
			timerOn = true;
			glutTimerFunc(10, onTimer, value);
		}
		break;
	case MENU_TIMEROFF:
		timerOn = false;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);              // inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // init double buffer

	glutInitWindowSize(600, 600);       // nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 100);   // nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("Shader");			// vytvoreni okna
	glutDisplayFunc(onDisplay);         // registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);         // registrace funkce volane pri zmene velikosti aktualniho okna

	int idSub = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_EXITOK);
	glutAddMenuEntry("Ne", MENU_EXITNO);

	int idTimerSub = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_TIMERON);
	glutAddMenuEntry("Ne", MENU_TIMEROFF);

	glutCreateMenu(onMenu);
	glutAddSubMenu("Timer", idTimerSub);
	glutAddSubMenu("Konec", idSub);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	onInit();							// (rucne napsana) inicializacni funkce
	glutMainLoop();                     // nekonecna smycka, interakce uz jen pomoci event handleru

	return 0;                           // ukonceni programu
}

