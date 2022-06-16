#include <gl\freeglut.h> 
#include <iostream> 
#include <ctime> 
#include <cmath> 




static GLuint triangulo, circulo_ext, puntos;
static float segundos  = 0, minutos  = 0, movimiento_sec = 0, vuelta = 0;
int grados_manilla_sec_min = 6;
time_t horas, minutoss, segundoss;
float r1 = 1.0, r2 = 0.7, punto_R = 1, radio = 1, PI = 3.14159265359;


void init()

{

	// CÍRCULO EXTERIOR
	
	circulo_ext = glGenLists(1);
	glNewList(circulo_ext, GL_COMPILE);

	
	glBegin(GL_POLYGON);
	
	for (int i = 0; i <= 100; i++) {	
		glVertex3f(1 * cos(i * 2 * 3.1415926 / 100), 1 * sin(i * 2 * 3.1415926 / 100), 0.0);
		glColor3f(0.3 / 50 * i, 0.4 - 0.2 / 50 * i, 0.9 - 0.4 / 50 * i);
		}
	glEnd();	
	glEndList();



   // TRIANGULO 
	triangulo = glGenLists(1);
	glNewList(triangulo, GL_COMPILE);

    glRotatef(vuelta, 0, 0, 1);
	glBegin(GL_TRIANGLE_STRIP);
	
	for (int i = 0; i < 5; i++) {
		glVertex3f(r1 * sin(i * 2 * PI / 3), r1 * cos(i * 2 * PI / 3), 0.0); // triangulo grande
		glVertex3f(r2 * sin(i * 2 * PI / 3), r2 * cos(i * 2 * PI / 3), 0.0); // para hacer hueco dentro del triangulo
	}
	glEnd(); 
	glEndList();


	// PUNTOS "engranaje" 
	puntos = glGenLists(1);
	glNewList(puntos, GL_COMPILE);

	glBegin(GL_POINTS);
	;
	for (int i = 0; i < 60; i++) {
		glVertex3f(punto_R * sin(i * 2 * PI / 60), punto_R * cos(i * 2 * PI / 60), 0.0);
	}
	glEnd();
	glEndList();

	// Tamaño de los puntos
	glEnable(GL_POINT_SIZE);



	// Tiempo
	time_t tiempo = time(NULL);
	horas = (tiempo / 3600) % 24;
	minutoss = (tiempo / 60) % 60;
	segundoss = tiempo % 60;

	

	glEnable(GL_DEPTH_TEST);

	}



void display()
{

	//Ligero color azul como fondo

	glClearColor(0.8, 1, 1, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Perspectiva frontal
	gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);

	// Círculo interior

	glPushMatrix();
	
	glRotatef(vuelta ,0,0,1);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 60; i++) {
		glColor3f(0.2 - 0.2 /50  * i, 0.4 - 0.5 / 50 * i, 0.6 - 0.9 / 50 * i);
		glVertex3f(0.3 * cos(i * 2 * 3.1415926 / 50), 0.3 * sin(i * 2 * 3.1415926 / 50), 0.0);
	

	}
	
	glEnd();
	glCallList(circulo_ext);
	glPopMatrix();
	
	

	// TRIANGULO 

	
	glPushMatrix();
	glScalef(0.3, 0.3, 0.25); // Ajustamos los triangulos al círculo interno
	glRotatef(vuelta, 0, 1, 0); // Rotacion constante sobre eje y de los trigangulos

	int vueltatriang = 0;
	
	for (int i = 0; i < 15; i++) { 
		glPushMatrix();
	    glRotatef(vueltatriang, 0, 1, 0); // Rotación en y
		glColor3f(0.2 - 0.1 / 4 * i, 0.7 - 0.5 / 4 * i, 0.7 - 0.9 / 4 * i); // Color degradado
		glCallList(triangulo);
		glPopMatrix();
		vueltatriang += 40;
	}
	glPopMatrix();



	// movimiento PUNTOS
	
	glPushMatrix();
	glRotatef(vuelta / 8, 0,0,1);
	glPointSize(20);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 100; i++) {	

			glVertex3f(0.0001 * cos(i * 2 * 3.1415926 / 50), 0.0001 * sin(i * 2 * 3.1415926 / 50), 0.0);

		
	}

	glEnd();
	
	glColor3f(0.0, 0.0, 0.0);
	glCallList(puntos);
	glPopMatrix();




	// movimiento manilla HORAS

	glPushMatrix();
	glColor3f(0, 0.5, 0.5);
	// Movimiento en si
	glRotatef((int)(movimiento_sec / (360 * 60 )) * 30, 0, 0, -1);
	// Colocar la manilla donde toca, tenemos que sumar 1 para que coincida con la hora española
	glRotatef((30 * (horas + 1)) + ((6 * minutoss * 30) / 360) - 90, 0, 0, -1);
	glTranslatef(0.25, 0, 0);
	glScalef(0.4, 0.04, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	// MINUTOS

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	// El movimiento en si
	glRotatef((int)(movimiento_sec / 360) *6, 0, 0, -1);
	// Colocar la manilla donde toca
	glRotatef((6 * minutoss) + ((6 * segundoss * 6) / 360) - 90 , 0, 0, -1);
	glTranslatef(0.4, 0, 0);
	glScalef(1.1, 0.01, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	//SEGUNDOS
	glPushMatrix();
	glColor3f(0.6, 2.0, 1.0);	
	// El movimiento en si
	glRotatef((int)movimiento_sec %360, 0, 0, -1);
	// Colocar la manilla donde toca
	glRotatef((6 * segundoss) -  90, 0, 0, -1);
	glTranslatef(0.4, 0, 0);
	glScalef(1, 0.01, 0.01);
	glutSolidCube(1);
	glPopMatrix();


	glutSwapBuffers();
}

void continuo()

{
	// Tiempo inicial
	static int antes = glutGet(GLUT_ELAPSED_TIME);

	// Tiempo actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	// Tiempo transcurrido convertido a segundos
	float tiempo_transcurrido = (ahora - antes) / 1000.0f;

	// 10 vueltas equivalen a una hora (3600 segundos)
	vuelta += 360 * tiempo_transcurrido;
	
	// Si ya ha avanzado un segundo, actualizamos variable y calculamos el movimiento
	if ((int)(vuelta / (360)) > segundos ) {
		segundos  = (int)vuelta / 360;
		movimiento_sec = segundos  * grados_manilla_sec_min;
	}

	
	antes = ahora;
	glutPostRedisplay();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	
	glClear(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Ajustar al tamaño de la ventana
	gluPerspective(55, (float)w / h, 1, 10);
}


void onTimer(int tiempo)
{
	glutTimerFunc(tiempo, onTimer, tiempo);
	continuo();
}

void main(int argc, char** argv)

{
	
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutCreateWindow("Reloj 3D"); 
	init();
	std::cout <<"Funcionando..." << std::endl; 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 

	
	// Actualización con tasa 60 FPS
	glutTimerFunc(1000 / 60, onTimer, 1000 / 60);
	
	glutMainLoop(); 
}