
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

/* Definicion de macros*/
#define PAINT 0
#define COMPLETO 1
#define LINEAL 2
#define CUADRATICO 3

/* Definicion de las funciones a utilizar*/
// Manejo de eventos
void mouse(int, int, int, int);
void display();
void myReshape(GLsizei, GLsizei);

//inicializacion
void myinit();  

//  Menus
void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void diametro_menu(int);
int pick(int, int);

//  Definicion de las funciones creadas por ti. Se pueden agregar funciones auxiliares
void print_matriz(double**);

void generate_linear_mask();
void generate_cuadratic_mask();
void generate_constant_mask();

void draw_linear_brush(int, int);
void draw_cuadratic_brush(int, int);
void draw_constant_brush(int, int);
void inicializa();
void pinta(int x, int y);

/* Variables globales */

GLsizei wh = 800, ww = 800;  // Tama;o inicial de la ventana
int draw_mode = 0;  // modo de dibujo
int rx, ry;  // raster pos
double diametro = 10;  // Guarda el diametro del airbrush
double radio = 0;  // Variable extra

// Color con el que se dibujara
GLfloat r = 1, g = 1, b = 1, cons = 1;
bool presionado = false;
bool generada_mascara_lineal = true;
bool generada_mascara_constante = false;
bool generada_mascara_cuadratica = false;

//Referencia a la mascara actual de valores alfa
double **current_alfa_mask;

/* Funcion que imprime la matriz con los valores alfa
* Se debe llamar cada vez que se calcula la matriz */
void print_matriz(double** matriz) {
	int aux = diametro - 1;
	for (int i = 0; i < diametro; i++) {
		for (int j = 0; j < diametro; j++) {
			printf("%f \t|", current_alfa_mask[i][j]);
			if (j == aux) {
				printf("\n");
			}
		}
	}
}

void pinta(int x, int y) {
	int i, j, pos_x, pos_y;
	int mitad = (int)radio;
	int aux1 = x - radio, aux2 = y - radio;
	for (i = 0; i < diametro; i++) {
		pos_x = aux1 + i;
		for (j = 0; j < diametro; j++) {
			pos_y = aux2 + j;
			glBegin(GL_POINTS);
			glColor4f(r, g, b, current_alfa_mask[i][j]);
			glVertex2f(pos_x, pos_y);
			glEnd();
		}

	}
	glFlush();
}

/* Calcula los valores alfa de la mascara lineal y asigna la referencia
* a current_alfa_mask */
void generate_linear_mask() {
	inicializa();
	int i, j;
	int mitad = (int)radio;
	current_alfa_mask[mitad][mitad] = 1.0;
	double dif = 1 / radio;
	int pos_aux;
	bool es_par = radio - mitad == 0;
	int diametro_aux = (int)diametro - 1;
	double valor, hipotenusa;
	if (es_par) {
		int diametro_aux = (int)diametro;
		current_alfa_mask[mitad][mitad - 1] = 1.0;
		current_alfa_mask[mitad - 1][mitad] = 1.0;
		int x = 1;
		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			valor = 1 - (x * dif);
			if (i != 0) {
				current_alfa_mask[i - 1][mitad] = valor;
				current_alfa_mask[mitad][i - 1] = valor;
				current_alfa_mask[pos_aux][mitad] = valor;
				current_alfa_mask[mitad][pos_aux] = valor;
			}
			x++;
		}

	}
	else {

		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			current_alfa_mask[mitad][i] = current_alfa_mask[mitad][i + 1] - dif;
			current_alfa_mask[i][mitad] = current_alfa_mask[mitad][i];
			current_alfa_mask[pos_aux][mitad] = current_alfa_mask[mitad][i];
			current_alfa_mask[mitad][pos_aux] = current_alfa_mask[mitad][i];
		}

	}

	for (i = 1; i < radio; i++) {
		for (j = 1; j < radio; j++) {
			hipotenusa = sqrt(pow(i, 2) + pow(j, 2));
			valor = 1 - (hipotenusa * dif);
			if (valor > 0) {
				current_alfa_mask[mitad + i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad - j] = valor;
				current_alfa_mask[mitad + i][mitad - j] = valor;
			}
		}
	}

	print_matriz(current_alfa_mask);
}

/* Calcula los valores alfa de la mascara cuadratica y asigna la referencia
* a current_alfa_mask */
void generate_cuadratic_mask() {
	inicializa();
	int i, j;
	int mitad = (int)radio;
	bool es_par = radio - mitad == 0;
	int diametro_aux = (int)diametro - 1;
	current_alfa_mask[mitad][mitad] = 1.0;
	double dif = 1 / radio;
	int x = 1;
	int pos_aux;
	double valor, hipotenusa;
	if (es_par) {
		int diametro_aux = (int)diametro;
		current_alfa_mask[mitad][mitad - 1] = 1.0;
		current_alfa_mask[mitad - 1][mitad] = 1.0;
		int x = 1;
		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			valor = 1 - (pow(x, 2) *dif);
			if (i != 0 && valor> 0) {
				current_alfa_mask[i - 1][mitad] = valor;
				current_alfa_mask[mitad][i - 1] = valor;
				current_alfa_mask[pos_aux][mitad] = valor;
				current_alfa_mask[mitad][pos_aux] = valor;
			}
			x++;
		}

	}
	else {
		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			valor = 1 - (pow(x, 2) *dif);
			if (valor> 0) {
				current_alfa_mask[mitad][i] = valor;
				current_alfa_mask[i][mitad] = current_alfa_mask[mitad][i];
				current_alfa_mask[pos_aux][mitad] = current_alfa_mask[mitad][i];
				current_alfa_mask[mitad][pos_aux] = current_alfa_mask[mitad][i];
			}
			x++;

		}
	}

	for (i = 1; i < radio; i++) {
		for (j = 1; j < radio; j++) {
			hipotenusa = sqrt(pow(i, 2) + pow(j, 2));
			valor = 1 - (pow(hipotenusa, 2) *dif);
			if (valor > 0) {
				current_alfa_mask[mitad + i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad - j] = valor;
				current_alfa_mask[mitad + i][mitad - j] = valor;
			}
		}
	}
	print_matriz(current_alfa_mask);
}

/* Calcula los valores alfa de la mascara constante y asigna la referencia
* a current_alfa_mask */
void generate_constant_mask() {
	inicializa();
	int i, j;
	int mitad = (int)radio;
	bool es_par = radio - mitad == 0;
	int diametro_aux = (int)diametro - 1;
	current_alfa_mask[mitad][mitad] = 1.0;
	double dif = 1 / radio;
	double constante = 0;
	int pos_aux;
	double valor, hipotenusa;
	if (es_par) {
		int diametro_aux = (int)diametro;
		current_alfa_mask[mitad][mitad - 1] = 1.0;
		current_alfa_mask[mitad - 1][mitad] = 1.0;
		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			valor = 1 - constante;
			if (i != 0) {
				current_alfa_mask[i - 1][mitad] = valor;
				current_alfa_mask[mitad][i - 1] = valor;
				current_alfa_mask[pos_aux][mitad] = valor;
				current_alfa_mask[mitad][pos_aux] = valor;
			}
		}

	}
	else {

		for (i = mitad - 1; i >= 0; i--) {
			pos_aux = diametro_aux - i;
			current_alfa_mask[mitad][i] = 1 - constante;
			current_alfa_mask[i][mitad] = current_alfa_mask[mitad][i];
			current_alfa_mask[pos_aux][mitad] = current_alfa_mask[mitad][i];
			current_alfa_mask[mitad][pos_aux] = current_alfa_mask[mitad][i];
		}

	}

	for (i = 1; i < radio; i++) {
		for (j = 1; j < radio; j++) {
			hipotenusa = sqrt(pow(i, 2) + pow(j, 2));
			valor = 1 - (hipotenusa * dif);
			if (valor > 0) {
				valor = 1 - constante;
				current_alfa_mask[mitad + i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad + j] = valor;
				current_alfa_mask[mitad - i][mitad - j] = valor;
				current_alfa_mask[mitad + i][mitad - j] = valor;
			}
		}
	}
	print_matriz(current_alfa_mask);
}

void inicializa() {
	int i, j;
	free(current_alfa_mask);
	current_alfa_mask = (double**)malloc(sizeof(double*) * diametro);
	for (i = 0; i < diametro; i++) {
		current_alfa_mask[i] = (double*)malloc(sizeof(double) * diametro);
	}
	for (i = 0; i < diametro; i++) {
		for (j = 0; j < diametro; j++) {
			current_alfa_mask[i][j] = 0.0;
		}
	}
}

/* Se ejecuta cuando se va a pintar sobre la pantalla*/
void draw_linear_brush(int x, int y) {
	y = wh - y;
	pinta(x, y);
}

/* Se ejecuta cuando se va a pintar sobre la pantalla*/
void draw_cuadratic_brush(int x, int y) {
	y = wh - y;
	pinta(x, y);
}

/* Se ejecuta cuando se va a pintar sobre la pantalla*/
void draw_constant_brush(int x, int y) {
	y = wh - y;
	pinta(x, y);

}

/* Se ejecuta cada vez que la venta cambia de tama;o */
void myReshape(GLsizei w, GLsizei h) {
	// Regresa al tama;o, pero aun asi borra :(
	glutReshapeWindow(ww, wh);
}
/* Funcion de inicializacion */
void myinit() {
	glViewport(0, 0, ww, wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);

	glClearColor(1.0, 1.0, 1.0, 1.0); //casi blanco
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	display();
}

/* Funcion que maneja los clicks con el mouse*/
void mouse(int btn, int state, int x, int y) {
	int where;  // nos dira que opcion es elegida
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {  // Click con el boton izquierdo
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		where = pick(x, y);  // where hace la llamada a pick
		printf("entre");
		printf("la opcion es %d\n", draw_mode);
		glColor3f(r, g, b); // Se carga el color
		if (where != PAINT) {
			// Si no se va a pintar, entonces se elige una brocha
			draw_mode = where;
		}
		else {
			presionado = true;
			// Se pinta con el modo de dibujo seleccionado previamente
			switch (draw_mode) {
			case(COMPLETO):
				// LLamada a la respectiva funcion
				if (!generada_mascara_constante) {
					generate_constant_mask();
					generada_mascara_constante = true;
					generada_mascara_cuadratica = false;
					generada_mascara_lineal = false;
				}
				draw_constant_brush(x, y);
				break;

			case(LINEAL):
				// LLamada a la respectiva funcion
				if (!generada_mascara_lineal) {
					generate_linear_mask();
					generada_mascara_constante = false;
					generada_mascara_cuadratica = false;
					generada_mascara_lineal = true;
				}

				draw_linear_brush(x, y);
				break;

			case (CUADRATICO):
				// LLamada a la respectiva funcion
				if (!generada_mascara_cuadratica) {
					generate_cuadratic_mask();
					generada_mascara_constante = false;
					generada_mascara_cuadratica = true;
					generada_mascara_lineal = false;
				}
				draw_cuadratic_brush(x, y);
				break;
			}
		}//end_else
		glPopAttrib();
		glFlush();
	}
	else if (state == GLUT_UP) {
		presionado = false;
	}
}

void mouseMove(int x, int y) {
	if (presionado) {
		switch (draw_mode) {
		case(COMPLETO):
			draw_constant_brush(x, y);
			break;

		case(LINEAL):
			// LLamada a la respectiva funcion
			draw_linear_brush(x, y);
			break;

		case (CUADRATICO):
			// LLamada a la respectiva funcion
			draw_cuadratic_brush(x, y);
			break;
		}
	}

}

/* Funcion que gestiona los clicks en pantalla */
int pick(int x, int y) {
	y = wh - y;  // Correccion de coordenadas
	if (y < wh - ww / 10) return PAINT;
	else if (x < ww / 10) return COMPLETO;
	else if (x < ww / 5) return LINEAL;
	else if (x < 3 * ww / 10) return CUADRATICO;
	else return 0;
}

/*  Pinta la caja en pantalla */
void screen_box(int x, int y, int s) {
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

/* Menus para el mouse */

void right_menu(int id) {
	if (id == 1) {
		exit(0);
	}
	else {
		glClearColor(1.0, 1.0, 1.0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		display();
	}
}

void middle_menu(int id) {

}

void color_menu(int id) {
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 5) { r = 0.0; g = 0.0; b = 0.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 0.5; b = 1.0; }
	else if (id == 8) { r = 0.5; g = 1.0; b = 1.0; }
	// Agregar mas colores para que en total sean 8
}

void diametro_menu(int id) {
	if (id == 1) {
		radio = diametro;
		diametro = 2 * diametro;
		//recalcular la matriz
	}
	else if (id == 2 && diametro > 1) {
		diametro = diametro / 2;
		radio = radio / 2;
		//recalcular la matriz
	}
	else if (id == 3) {
		diametro = 5;
		radio = diametro / 2;
	}
	else if (id == 4) {
		diametro = 11;
		radio = diametro / 2;
	}
	else if (id == 5) {
		diametro = 80;
		radio = diametro / 2;
	}
	else if (id == 6) {
		diametro = 101;
		radio = diametro / 2;
	}
	else if (id == 7) {
		diametro = 300;
		radio = diametro / 2;
	}
	else if (id == 8) {
		diametro = 501;
		radio = diametro / 2;

	}
	if (generada_mascara_constante) { generate_constant_mask(); }
	else if (generada_mascara_cuadratica) { generate_cuadratic_mask(); }
	else { generate_linear_mask(); }
	//Gestionar los demas items
}

/* Funcion de display */
void display() {

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glClearColor (0.8, 0.8, 0.8, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);

	/* Se pintan los cuadrados. */
	// constante
	glColor3f(1.0, 1.0, 1.0);
	screen_box(0, wh - ww / 10, ww / 10);
	// lineal
	glColor3f(1.0, 0.0, 0.0);
	screen_box(ww / 10, wh - ww / 10, ww / 10);
	//cuadratico
	glColor3f(0.0, 1.0, 0.0);
	screen_box(ww / 5, wh - ww / 10, ww / 10);

	/* Cambiar los dibujos por algo mas ilustrativo
	Se pueden generar letras con:
	*/
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();
	
	glFlush();
	glPopAttrib();
}

int main(int argc, char** argv) {
	// menus
	int c_menu, r_menu;
	//Se prepaa la mascara lineal para ponerla por defecto

	radio = diametro / 2;
	generate_linear_mask();
	//draw_mode = LINEAL;

	// Ventana
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("square");
	glutDisplayFunc(display);
	glutMotionFunc(mouseMove);

	/* Menus */
	// Se le da la funcion que gestiona el menu
	c_menu = glutCreateMenu(color_menu);
	// Se dan los Items del menu
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Fuchsia", 4);
	glutAddMenuEntry("Black", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("Magenta", 7);
	glutAddMenuEntry("Sky blue", 8);

	//Se genera otro menu
	r_menu = glutCreateMenu(diametro_menu);
	glutAddMenuEntry("Aumenta diametro", 1);
	glutAddMenuEntry("Disminuye diametro", 2);
	glutAddMenuEntry("diametro = 5", 3);
	glutAddMenuEntry("diametro = 11", 4);
	glutAddMenuEntry("diametro = 80", 5);
	glutAddMenuEntry("diametro = 101", 6);
	glutAddMenuEntry("diametro = 300", 7);
	glutAddMenuEntry("diametro = 501", 8);

	glutCreateMenu(right_menu);
	glutAddMenuEntry("salir", 1);
	glutAddMenuEntry("limpiar", 2);

	//El ultimmo menu generado se pone en el boton derecho del mouse
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Se crea otro menu al que se a;aden como submenus los anteriores
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colores", c_menu);
	glutAddSubMenu("Tama;o del diametro", r_menu);
	// Se asigna al click medio del mouse
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	// inicializacion y asignacion de funciones
	myinit();
	glutReshapeFunc(myReshape);
	glutMouseFunc(mouse);
	// Soporte para manejar los canales alfa
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glutMainLoop();
}