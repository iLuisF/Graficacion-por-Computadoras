
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <glut.h>

int flechas[2];
GLint factor = 10;
GLushort patron = 0x5555;
int contador;
double* color;
//Medida de cada lado del cubo.
float lado_cubo;
float radio;
float interno;
float externo;

//Definimos las funciones.
void dibujarCubo();
void dibujarEsferas();
void dibujarToros();
void dibujarEjes();

/* En esta sección se indicarán los parámetros de
inicialización de OpenGL (estado de buffers,.)*/
void init(void)
{
	//se habilita la utilización de cálculos de profundidad.
	glEnable(GL_DEPTH_TEST);
	//Se limpia el buffer de color y se define por defecto el color negro.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	contador = 0;
	lado_cubo = 1.0;
	radio = 0.8;
	interno = 0.2;
	externo = 0.7;
	color = (double*)calloc(9, sizeof(double));
	color[0] = 1.0;
	color[4] = 1.0;
	color[8] = 1.0;
}

/* En esta sección, se indicarán las características de OpenGL
incluyendo el código necesario para representar los objetos
en pantalla */
void display(void)
{	
	/*Esta función resetea los buffer de color y profundidad,
	eliminando posibles contenidos de ventanas anteriores*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	/* Esta función carga como matriz de proyección la matriz identidad*/
	glLoadIdentity();
	/*En OpenGL, los cálculos de rotaciones, texturas, traslaciones, se
	realizan mediante un cálculo de matrices. Esta función crea una copia
	de la matriz de modelado y la empuja una posición en la pila de matrices*/
	glPushMatrix();
	/*Esta función especifica el color en formato RGB, el siguiente es negro.*/
	glColor4f(1.0, 1.0, 1.0, 1.0); 
	/*Esta función realiza una traslación del origen de coordenadas al
	punto (0,0,-30)*/
	glTranslatef(0.0, 0.0, -30);
	glRotatef(35, 1.0, 0.0, 0.0);
	glRotatef(135, 0.0, 1.0, 0.0);
	dibujarEjes();
	dibujarCubo();
	dibujarEsferas();
	dibujarToros();
	/*Esta función se encarga de intercambiar el buffer posterior con el
	buffer anterior, siendo necesaria porque se ha definido que se trabaje
	con un doble buffer. La utilización de este doble buffer evita
	parpadeos en la generación de la imagen*/
	glutSwapBuffers();
}

/*En esta sección, se indicará el código a ejecutar cuando se
produce una redimensión de la ventana. También es utilizada
para definir el área de proyección de la figura en la ventana*/
void reshape(int w, int h)
{	
	/*Esta función indica la porción de la ventana donde OpenGL
	podrá dibujar*/
	glViewport(0, 0, w, h);
	/*Esta función especifica la matriz de proyección como matriz sobre
	la que se van a realizar las transformaciones. OpenGL dispone de tres
	tipos de matrices: GL_PROJECTION, GL_MODELVIEW y GL_TEXTURE */
	glMatrixMode(GL_PROJECTION);
	/* Esta función carga como matriz de proyección la matriz identidad*/
	glLoadIdentity();
	/* Esta función opera sobre la matriz de proyección y define el
	ángulo del campo de visión en sentido vertical (en grados), la
	relación entre la altura y anchura de la figura (aspecto), el plano
	más cercano a la cámara y el más lejano respectivamente, se les llama
	planos de corte y son los que se encargan de acotar el volúmen de
	visualización por delante y detrás de nuestro espacio de trabajo*/
	if (h == 0) {		
		gluPerspective(80, (float)w, 1.0, 5000.0);
	}
	else {
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	}
	/*Esta funcion específica la matriz de modelado como matriz por
	la que se van a realizar las transformaciones*/
	glMatrixMode(GL_MODELVIEW);
	/* Esta función carga como matriz de proyección la matriz identidad*/
	glLoadIdentity();
}

/* En esta sección, se indicarán las tareas a realizar ante
un evento del teclado. Se ha de tener presente que todos los
eventos de teclado no producen un redibujado de la pantalla,
por lo que es conveniente forzar este redibujado con funciones 
como glutPostRedisplay(). Esc - salir, 1 - cambiar color del eje X, 
2 - cambiar color del eje Y, 3 - cambiar color del eje Z*/
void keyboard(unsigned char key, int x, int y)
{	
	switch (key) {
	case 27: /* tecla de Escape */
		exit(0);
		break;
	case '1':
		if (color[0] == 1.0) {
			if (color[1] == 1.0) {
				if (color[2] == 1.0) {
					color[0] = 0.0;
					color[1] = 0.0;
					color[2] = 0.0;
				}
				else {
					color[2] = 1.0;
				}
			}
			else {
				color[1] = 1.0;
			}
		}
		else {
			color[0] = 1.0;
		}
		break;
	case '2':
		if (color[3] == 1.0) {
			if (color[4] == 1.0) {
				if (color[5] == 1.0) {
					color[3] = 0.0;
					color[4] = 1.0;
					color[5] = 0.0;
				}
				else {
					color[5] = 1.0;
				}
			}
			else {
				color[4] = 1.0;
			}
		}
		else {
			color[3] = 1.0;
		}
		break;
	case '3':
		if (color[6] == 1.0) {
			if (color[7] == 1.0) {
				if (color[8] == 1.0) {
					color[6] = 0.0;
					color[7] = 0.0;
					color[8] = 1.0;
				}
				else {
					color[8] = 1.0;
				}
			}
			else {
				color[7] = 1.0;
			}
		}
		else {
			color[6] = 1.0;
		}
		break;
	default:
		break;
	}
	display();
}

/* En esta sección, se indicarán las tareas a realizar ante
un evento de ratón Se ha de tener presente que todos los
eventos de ratón no producen un redibujado de la pantalla,
por lo que es conveniente forzar este redibujado con
funciones como glutPostRedisplay(). Click derecho - aumenta tamaño,
click izquierdo diminuye tamaño*/
void mouse(int button, int state, int x, int y)
{
	//Disminuye el tamaño del cubo.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{		
		if (contador != -1) {
			contador--;
			lado_cubo -= 0.5;
			radio -= 0.4;
			interno -= 0.1;
			externo -= 0.2;
		}
	}
	//Aumenta el tamaño del cubo.
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		contador++;
		lado_cubo += 0.5;
		radio += 0.4;
		interno += 0.1;
		externo += 0.2;
	}
}

/*
* Dibuja el cubo en el centro.
*/
void dibujarCubo() {
	glColor3f(lado_cubo, lado_cubo, 0.0f);
	//Cara frontal
	glBegin(GL_QUADS);
	glVertex3f(-lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, lado_cubo);
	glVertex3f(-lado_cubo, lado_cubo, lado_cubo);
	glEnd();
	//Cara trasera
	glBegin(GL_QUADS);
	glVertex3f(lado_cubo, -lado_cubo, -lado_cubo);
	glVertex3f(-lado_cubo, -lado_cubo, -lado_cubo);
	glVertex3f(-lado_cubo, lado_cubo, -lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, -lado_cubo);
	glEnd();
	//Cara derecha
	glBegin(GL_QUADS);
	glVertex3f(-lado_cubo, -lado_cubo, -lado_cubo);
	glVertex3f(-lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(-lado_cubo, lado_cubo, lado_cubo);
	glVertex3f(-lado_cubo, lado_cubo, -lado_cubo);
	glEnd();
	//Cara izquierda
	glBegin(GL_QUADS);
	glVertex3f(lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(lado_cubo, -lado_cubo, -lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, -lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, lado_cubo);
	glEnd();
	//Cara superior
	glBegin(GL_QUADS);
	glVertex3f(-lado_cubo, lado_cubo, lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, lado_cubo);
	glVertex3f(lado_cubo, lado_cubo, -lado_cubo);
	glVertex3f(-lado_cubo, lado_cubo, -lado_cubo);
	glEnd();
	//Cara inferior
	glBegin(GL_QUADS);
	glVertex3f(lado_cubo, -lado_cubo, -lado_cubo);
	glVertex3f(lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(-lado_cubo, -lado_cubo, lado_cubo);
	glVertex3f(-lado_cubo, -lado_cubo, -lado_cubo);
	glEnd();
}

/**
* Dibuja los ejes X, Y, Z.
*/
void dibujarEjes() {
	//Activar el punteado de líneas
	glEnable(GL_LINE_STIPPLE); 
	glLineStipple(factor, patron);
	int i = 0;
	float inicio = -100.0f;	
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 100; i++) {
		inicio += 2;
		//Eje X.
		glVertex3f(inicio, 0.0f, 0.0f);
	}
	glEnd();
	inicio = -80;
	glColor3f(color[3], color[4], color[5]);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 80; i++) {
		inicio += 2;
		//Eje Y.
		glVertex3f(0.0f, inicio, 0.0f);
	}
	glEnd();
	inicio = -100;
	glColor3f(color[6], color[7], color[8]);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 100; i++) {
		inicio += 2;
		//Eje z.
		glVertex3f(0.0f, 0.0f, inicio);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

/*
* Dibuja 3 esferas, una en cada eje y el color de estas corresponde
* al eje en la que se encuentra.
*/
void dibujarEsferas() {
	float distancia = lado_cubo + 2 * radio - 0.2;
	glTranslatef(-lado_cubo, lado_cubo, distancia);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(radio, 20, 20);

	glTranslatef(lado_cubo, -lado_cubo, -distancia);

	glTranslatef(distancia, lado_cubo, -lado_cubo);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(radio, 20, 20);

	glTranslatef(-distancia, -lado_cubo, lado_cubo);

	glTranslatef(-lado_cubo, -distancia, -lado_cubo);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidSphere(radio, 20, 20);

	glTranslatef(lado_cubo, distancia, lado_cubo);

}

/*
* Dibuja 3 toros, uno en cada eje y el color de estos corresponde
* al eje en el que se encuentra.
*/
void dibujarToros() {
	float distancia = lado_cubo + 2 * interno + 2 * externo + 2;

	glTranslatef(-lado_cubo, lado_cubo, -distancia);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidTorus(interno, externo, 20.0, 20.0);

	glTranslatef(lado_cubo, -lado_cubo, distancia);

	glTranslatef(-distancia + 1, -lado_cubo, lado_cubo);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidTorus(interno, externo, 20.0, 20.0);

	glTranslatef(distancia - 1, lado_cubo, -lado_cubo);

	glTranslatef(-lado_cubo, distancia, -lado_cubo);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidTorus(interno, externo, 20.0, 20.0);

	glTranslatef(lado_cubo, -distancia, lado_cubo);
}

int main(int argc, char** argv)
{
	//Inicializa GLUT.
	glutInit(&argc, argv);
	//Modo en que se dibuja la ventana.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//Tamaño de la ventana en pixeles.
	glutInitWindowSize(800, 600);
	//Lugar donde va a estar la ventana en la pantalla.
	glutInitWindowPosition(100, 100);
	//Se crea la ventana.
	glutCreateWindow("Práctica 1");
	//Se inicicializan los parámetros propios de openGL.
	init();	
	glutDisplayFunc(display);
	/*La función que se pasa como parametro se llamara cada vez que GLUT determine
	que la ventana debe ser redibujada.*/
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);	
	//Esta función cede el control de flujo de programa a GLUT.
	glutMainLoop(); 

	return 0;
}
