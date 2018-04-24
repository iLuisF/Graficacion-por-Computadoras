
#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <math.h>

#define MaxPuntos	2000
#define MaxPoligonos    4000

struct {
	GLfloat fVert1[3];
	GLfloat fVert2[3];
	GLfloat fVert3[3];
	GLfloat fNormal[3];
} Tri[MaxPoligonos];

GLfloat vertice[MaxPuntos][3];
GLfloat vnorms[MaxPuntos][3];
GLint   poli[MaxPoligonos][3];

GLfloat elevacion = 0.0f;
GLfloat azimitud = 0.0f;
GLfloat giro = 0.0f;

GLint	numPoligonos;
GLint   numVertices;

GLint   vertNorms = 0;
GLint   fill = 1;

void GargarPoligonos(char *nombre)
{
	int i;
	printf("%s", nombre);
	FILE *in = fopen(nombre, "r");
	if (!in)
		return;
	fscanf(in, "%d\n", &numVertices);
	for (i = 0; i<numVertices; i++)
		fscanf(in, "%f %f %f\n", &vertice[i+1][0], &vertice[i+1][1], &vertice[i+1][2]);
	fscanf(in, "%d\n", &numPoligonos);
	for (i = 0; i<numPoligonos; i++)
		fscanf(in, "%d %d %d\n", &poli[i][0], &poli[i][1], &poli[i][2]);
	fclose(in);
}

//Calcula el modulo de un vector
GLfloat Modulo(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat len;

	len = x * x + y * y + z * z;
	return (sqrt(len));
}

//Normaliza el vector a mudulo 1
GLvoid Normaliza(GLfloat *x, GLfloat *y, GLfloat *z)
{
	GLfloat len;

	len = Modulo(*x, *y, *z);
	len = 1.0 / len;
	(*x) *= len;
	(*y) *= len;
	(*z) *= len;
}

//Calcula el vector Normal a un poligono
GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[],
	GLfloat fVert3[], GLfloat *fNormalX,
	GLfloat *fNormalY, GLfloat *fNormalZ)
{
	GLfloat Qx, Qy, Qz, Px, Py, Pz;
	Px = fVert2[0] - fVert1[0];
	Py = fVert2[1] - fVert1[1];
	Pz = fVert2[2] - fVert1[2];
	Qx = fVert3[0] - fVert1[0];
	Qy = fVert3[1] - fVert1[1];
	Qz = fVert3[2] - fVert1[2];
	*fNormalX = Py * Qz - Pz * Qy;
	*fNormalY = Pz * Qx - Px * Qz;
	*fNormalZ = Px * Qy - Py * Qx;
}

void CalcularNormales(void)
{
	GLfloat fNormalX, fNormalY, fNormalZ;
	GLfloat sumx = 0.0, sumy = 0.0, sumz = 0.0;
	int shared = 0;
	GLint i, j;

	for (i = 0; i<numPoligonos; i++)
	{
		//Vertice 1
		j = poli[i][0];
		Tri[i].fVert1[0] = vertice[j][0];
		Tri[i].fVert1[1] = vertice[j][1];
		Tri[i].fVert1[2] = vertice[j][2];
		//Verice 2
		j = poli[i][1];
		Tri[i].fVert2[0] = vertice[j][0];
		Tri[i].fVert2[1] = vertice[j][1];
		Tri[i].fVert2[2] = vertice[j][2];
		//Vertice 3 
		j = poli[i][2];
		Tri[i].fVert3[0] = vertice[j][0];
		Tri[i].fVert3[1] = vertice[j][1];
		Tri[i].fVert3[2] = vertice[j][2];

		//Calcula el vector normal al poligono
		CalculateVectorNormal(Tri[i].fVert1, Tri[i].fVert2, Tri[i].fVert3, &fNormalX, &fNormalY, &fNormalZ);

		//nos retorna el vector unitario, es decir de modulo 1 
		Normaliza(&fNormalX, &fNormalY, &fNormalZ);

		//almacena los vectores normales, para cada poligono
		Tri[i].fNormal[0] = fNormalX;
		Tri[i].fNormal[1] = fNormalY;
		Tri[i].fNormal[2] = fNormalZ;
	};

	for (i = 0; i<numVertices; i++)
	{
		for (j = 0; j<numPoligonos; j++)
		{
			if (poli[j][0] == i || poli[j][1] == i || poli[j][2] == i || poli[j][3] == i)
			{
				sumx = sumx + Tri[j].fNormal[0];
				sumy = sumy + Tri[j].fNormal[1];
				sumz = sumz + Tri[j].fNormal[2];
				shared++;
			}
		}
		vnorms[i][0] = sumx / (float)shared;
		vnorms[i][1] = sumy / (float)shared;
		vnorms[i][2] = sumz / (float)shared;
		Normaliza(&vnorms[i][0], &vnorms[i][1], &vnorms[i][2]);
		sumx = 0.0;
		sumy = 0.0;
		sumz = 0.0;
		shared = 0.0;
	}
}

static void init(void)
{
	//defino el color y la posicion de la fuente de luz
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 0.0f, 0.0f, 1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	char nombreModelo[] = "model.DAT";
	GargarPoligonos(nombreModelo);
	CalcularNormales();

}

void polarView(GLfloat distance, GLfloat twist, GLfloat elevation, GLfloat azimuth)
{
	glTranslated(0.0, 0.0, -distance);
	glRotated(-twist, 0.0, 0.0, 1.0);
	glRotated(-elevation, 1.0, 0.0, 0.0);
	glRotated(azimuth, 0.0, 0.0, 1.0);
}

void display(void)
{
	GLfloat mat_ambient[] = { 0.0f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_diffuse[] = { 0.2f, 0.7f, 0.9f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLint i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	polarView(400.0f, giro, elevacion, azimitud);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

	glBegin(GL_TRIANGLES);
	for (i = 0; i<numPoligonos; i++)
	{
		if (!vertNorms)
			glNormal3fv(Tri[i].fNormal);
		if (vertNorms)
		{
			j = poli[i][0];
			glNormal3fv(vnorms[j]);
		};
		glVertex3fv(Tri[i].fVert1);
		if (vertNorms)
		{
			j = poli[i][1];
			glNormal3fv(vnorms[j]);
		};
		glVertex3fv(Tri[i].fVert2);
		if (vertNorms)
		{
			j = poli[i][2];
			glNormal3fv(vnorms[j]);
		};
		glVertex3fv(Tri[i].fVert3);
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat)w / (GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
	case 'X':
		elevacion = elevacion + 10.0f;
		glutPostRedisplay();
		break;
	case 'y':
	case 'Y':
		azimitud = azimitud + 10.0f;
		glutPostRedisplay();
		break;
	case 'z':
	case 'Z':
		giro = giro + 10.0f;
		glutPostRedisplay();
		break;
	case 'v':
	case 'V':
		vertNorms = !vertNorms;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		fill = !fill;
		if (fill)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_LIGHTING);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		};
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char **argv)
{
	glutInitWindowSize(300, 300);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST); /* Permite la eliminación de superficies ocultas */
	glutMainLoop();
	return 0;
}
