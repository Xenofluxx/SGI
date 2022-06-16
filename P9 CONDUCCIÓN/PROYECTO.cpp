#include <iostream> 
#include <gl\freeglut.h> 
#include <Utilidades.h>
#include <cmath> 
#include <sstream>
#include <ctime> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define PROYECTO "CONDUCCION"
#define PI 3.1415926
#define tasaFPS 60
#define tope_Particulas 2000
#define WCX	1000
#define WCY	1000


//Parte de iluminación
GLfloat carretera_difuso[] = { 0.8,0.8,0.8 };
GLfloat carretera_especular[] = { 0.3,0.3,0.3 };

GLfloat ambiental_Luna[] = { 0.05,0.05,0.05 };
GLfloat difusa_Luna[] = { 0.05,0.05,0.05 };
GLfloat especular_Luna[] = { 0.0,0.0,0.0 };
GLfloat posicion_Luna[4] = { 0, 10, 0, 0 };

GLfloat ambiental_Coche[] = { 0.2,0.2,0.2 };
GLfloat difusa_Coche[] = { 1.0,1.0,1.0 };
GLfloat especular_Coche[] = { 0.3,0.3,0.3 };


GLfloat ambiental_Farolas[] = { 0.0,0.0,0.0 };
GLfloat difusa_Farolas[] = { 0.5,0.5,0.2 };
GLfloat especular_Farolas[] = { 0.0,0.0,0.0 };


GLfloat dir_light1[] = { 0.0, -0.5, -0.7 };
GLfloat dir_farola[] = { 0, -1.0, 0 };

// X mueve de izquierda a derecha 
// positivo hacia la derecha
// Y es la concentración de la luz (
// Z mueve hacia arriba y hacia abajo
//negativo hacia delante
GLfloat posicionL1[] = { 0 ,0.7,0,1 };
// No tocar z ni última posición
GLfloat posicionL4[] = { 0,4,10,1 };
GLfloat posicionL2[] = { 0,4,-10,1 };
GLfloat posicionL3[] = { 0,4,-30.6,1 };
GLfloat posicionL5[] = { -15,4,-33,1 };
GLfloat posicionL6[] = { -27,4,-60,1 };
GLfloat posicionL7[] = { 20,4,36,1 };








float slowdown = 2.0;
float velocidad_ceniza = 2.0;
//indica como de alto o bajo se sitúa el origen
float pos_Inicio_ceniza = -10;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.01;
int i;
float accum = -10.0;

typedef struct {
	bool alive;
	float life;
	float fade;	
	float xpos;
	float ypos;
	float zpos;
	float vel;
	float gravity;
}particles;

particles par_sys[tope_Particulas];



// Texturas
static GLuint suelos, meta, coches, cartel, fondo, carretera;
// Restaremos y sumaremos 1/4 respecto a esto
static float angulo = 180;
// Grados posición visión
static int FOVY = 45;
time_t h, m, s;
static enum { SOLIDO, ALAMBRICO } alambrico_solido;
static enum { DIA, NOCHE } noche_Dia;
static enum { OFF, ON} niebla;
static enum {OFFl, ONl} ceniza;
static enum { DES, ACT } solidario;
static float velocidad = 0, giro_X = 0, giro_Z = -1, distancia_X = 0, distancia_Y = 1, distancia_Z = 0, avanzado = 0.0, alfa = 0;






void es_Noche() {
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// 
	//Para que sea posible ver las luces bien 
	glDisable(GL_BLEND);

	//Corregir color coche
	glClearColor(0.1, 0.1, 0.1, 1);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

}
//Cargamos las texturas

void cargar_Texturas() {
	glEnable(GL_TEXTURE_2D);


	glGenTextures(1, &suelos);
	glBindTexture(GL_TEXTURE_2D, suelos);
	loadImageFile((char*)"suelos.jpg");

	glGenTextures(1, &cartel);
	glBindTexture(GL_TEXTURE_2D, cartel);
	loadImageFile((char*)"cartel.jpg");


	glGenTextures(1, &fondo);
	glBindTexture(GL_TEXTURE_2D, fondo);
	loadImageFile((char*)"fondo.png");

	glGenTextures(1, &carretera);
	glBindTexture(GL_TEXTURE_2D, carretera);
	loadImageFile((char*)"carretera.jpg");

	glGenTextures(1, &meta);
	glBindTexture(GL_TEXTURE_2D, meta);
	loadImageFile((char*)"meta.png");


	glGenTextures(1, &coches);
	glBindTexture(GL_TEXTURE_2D, coches);
	loadImageFile((char*)"car.png");

	
}



void initParticles(int i) {
	
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	// Evita que se muevan todas a la vez
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 21) - 10;
	par_sys[i].ypos = 10.0;
	par_sys[i].zpos = (float)(rand() % 21) - 10;

	

	par_sys[i].vel = velocidad_ceniza;
	par_sys[i].gravity = -0.8;
}



void cargar_Luces() {
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);
	glEnable(GL_LIGHT7);
	 
	//LUZ DIRECCIONAL DE LA LUNA  LIGHT0	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiental_Luna);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa_Luna);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular_Luna);
	glLightfv(GL_LIGHT0, GL_POSITION, posicion_Luna);

	//LUZ FOCAL FARO  LIGHT1
		
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambiental_Coche);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa_Coche);
	glLightfv(GL_LIGHT1, GL_SPECULAR, especular_Coche);	
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);		
	glLightfv(GL_LIGHT1, GL_POSITION, posicionL1);	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_light1);	

	//LUZ FOCAL FAROLAS LIGHT2-LIGHT5
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT2, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);

	
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT3, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT4, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);
	
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT5, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);	

	

	glLightfv(GL_LIGHT6, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT6, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0);


	glLightfv(GL_LIGHT7, GL_AMBIENT, ambiental_Farolas);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, difusa_Farolas);
	glLightfv(GL_LIGHT7, GL_SPECULAR, especular_Farolas);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0);



	// Carretera

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, carretera_difuso);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, carretera_especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	


}



void dibujar_ceniza() {
	//glEnable(GL_LIGHTING);
	// glColorMaterial(GL_FRONT_AND_BACK,
	//	 GL_SPECULAR);
	float x, y, z;

	for (i = 0; i < tope_Particulas; i +=  1){
		if (par_sys[i].alive == true) {
			x = par_sys[i].xpos;
			y = par_sys[i].ypos;
			z = par_sys[i].zpos + pos_Inicio_ceniza;
			

			//Dibujamos los bloques de ceniza
			glBegin(GL_POLYGON);	
			
			glVertex3f(x - hailsize , y - hailsize, z + hailsize); 
			
			glVertex3f(x - hailsize , y + hailsize , z + hailsize );
			
			glVertex3f(x + hailsize , y + hailsize , z + hailsize); 
			
			glVertex3f(x + hailsize, y - hailsize , z + hailsize);
			glEnd();

			
			//Moverlas
			if (par_sys[i].ypos <= -10) {
				par_sys[i].vel = par_sys[i].vel * -1.0;
			}
			par_sys[i].ypos += par_sys[i].vel / (slowdown * 1000); 
			par_sys[i].vel += par_sys[i].gravity;

			// Desaparecer
			par_sys[i].life -= par_sys[i].fade;

			//Reaparecer
			if (par_sys[i].life < 0.0) {
				initParticles(i);
			}
		}
	}
}

void continuar_ceniza(){
	//Sirve para que se sigan dibujando mientras te mueves 
	//glEnable(GL_LIGHTING);
	//glColorMaterial(GL_FRONT_AND_BACK,
	//	GL_SPECULAR);
	int i, j;
	float x, y, z;	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(pan, 0.0, 2.0, 0.0);
	glRotatef(tilt, 1.0, 0.0, 0.0);	
	glBegin(GL_POLYGON);	
	for (i = -10; i + 1 < 11; i++) {
	
		for (j = -10; j + 1 < 11; j++) {
			;
			glVertex3f(distancia_X,
				distancia_Y, distancia_Z);
		
			glVertex3f(distancia_X,
				distancia_Y, distancia_Z);
	
			glVertex3f(distancia_X,
				distancia_Y, distancia_Z);
			
			glVertex3f(distancia_X,
				distancia_Y, distancia_Z);
		}

	}
glEnd();	
dibujar_ceniza();

}



void init(){
	glEnable(GL_DEPTH_TEST);	
	cargar_Texturas();
	cargar_Luces();
	

	cout << "	Flechas izquierda y derecha: mover coche hacia los lados\n";
	cout << "	Flechas arriba y abajo: se modifica la velocidad del coche\n";
	cout << "	s : Activa / desactiva un modelo simple en alámbrico\n";
	cout << "	l : Cambia entre modo diurno / nocturno\n";	
	cout << "	n : Cambia el estado de la niebla(on / off)\n";	
	cout << "	c : Muestra y esconde los elementos solidarios a la cámara, que son un coche y una barra de velocidad(on / off):\n";
	cout << "	g : Cambia el estado de la ceniza(on / off)\n";

	for (int particulas = 0; particulas < tope_Particulas; particulas++) {
		initParticles(particulas);
	}

}

void farolas() {


	glPushMatrix();	

	// Farola 1	
	glLightfv(GL_LIGHT2, GL_POSITION, posicionL2);	
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_farola);
	// Farola 2	
	glLightfv(GL_LIGHT3, GL_POSITION, posicionL3);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir_farola);
	// Farola 3	
	glLightfv(GL_LIGHT4, GL_POSITION, posicionL4);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir_farola);
	// Farola 4	
	glLightfv(GL_LIGHT5, GL_POSITION, posicionL5);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir_farola);
	// Farola 5
	glLightfv(GL_LIGHT6, GL_POSITION, posicionL6);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, dir_farola);
	// Farola 6

	glLightfv(GL_LIGHT7, GL_POSITION, posicionL7);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, dir_farola);

	glPopMatrix();
}

void pista() {

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//DESDE EL EJE DE COORDENADAS
	///   + DETRÁS Y DERECHA
	//	  - DELANTE E IZQUIEDRDA

	//					ARG1					ARG2					 ARG3
	//																
	//V				HORIZONTAL					Y(no tocar)					 VERTICAL
	//V																				
	//V			
	//V

	

	//CONECTAR V!

	// Quad inicial 
	GLfloat v0[3] = {-4,0,30 }, v1[3] = { 4,0,30 }, v2[3] = { 4,0,-30 }, v3[3] = {- 4,0,-30 };
	quad(v0, v1, v2, v3,10,15);		

	//Quad frontal izq

	GLfloat v4[3] = { 4,0,-30 } , v5[3] = { 4,0,-34 }, v6[3] = { -30,0,-34 }, v7[3] = { -30,0,-30 };
	quad(v4, v5, v6, v7, 10,15);

	// Quad trasero
	GLfloat v8[3] =  { 30, 0, 30 }, v9[3] = { -4,0,40 }, v10[3] = { -4,0,30 }, v11[3] = { 30,0,40 };
	quad(v10,v9, v11, v8, 10, 15);

	//Quad pegado al frontal izq

	GLfloat v12[3] ={ -30, 0, -30 }, v13[3] = { -25,0,-80 }, v14[3] = { -25,0,-34 }, v15[3] = { -30,0,-80 };
	quad(v12, v14, v13, v15, 10, 15);

	

	glPopMatrix();
}



void carteles1() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, cartel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	

	glPushMatrix();
	glLineWidth(5);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(2, 0, -20);
	glVertex3f(2, 4, -20);
	glVertex3f(-2, 4, -20);
	glVertex3f(-2, 0, -20);

	glEnd();
	glPopMatrix();



	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, 2, -20);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, -20);
	glTexCoord2f(1, 1);
	glVertex3f(2, 6, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-2, 6, -20);
	glEnd();
	
	glEnd();
	glPopMatrix();

	


}



void fondos() {

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, fondo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	

	glBegin(GL_QUAD_STRIP);
	float alfa = 2 * PI / 10.0;
	for (int i = 0; i <= 14; i++) {
		float ex = 300 * sin(i * alfa);
		float ez = 300 * cos(i * alfa);
		glTexCoord2f(i * alfa / (0.5 * PI), 0);
		glVertex3f(ex, -30, ez);
		glTexCoord2f(i * alfa / (0.5 * PI), 1);
		glVertex3f(ex, 180, ez);
	}
	glEnd();

	glPopMatrix();
}

void suelo() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, suelos);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-300, -0.01, 300);
	glTexCoord2f(1, 0);
	glVertex3f(300, -0.01, 300);
	glTexCoord2f(1, 1);
	glVertex3f(300, -0.01, -300);
	glTexCoord2f(0, 1);
	glVertex3f(-300, -0.01, -300);
	glEnd();
	glPopMatrix();

}


void coche() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, coches);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, -2);
	glTexCoord2f(1, 0);
	glVertex3f(1, 0, -2);
	glTexCoord2f(1, 1);
	glVertex3f(1, 0.4, -2);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 0.4, -2);
	glEnd();
	glPopMatrix();

}

void barra_Velocidad() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_POLYGON);

	if (velocidad>= 3.0) {
		glColor3f(0.0, 0.9, 0.4);
	}
	if (velocidad>= 5.5) {
		glColor3f(0.0, 0.9, 0.0);
	}
	if (velocidad>= 7) {
		glColor3f(0.4, 0.9, 0.0);
	}

	glTranslatef(-0.1, -8, 0);
	glVertex3f(1, 8, 0);
	glVertex3f(1, 0.75, -2.0);

	if (velocidad>= 3.0) {
		glColor3f(1.0, 0.5, 0.2);
	}
	if (velocidad>= 5.5) {
		glColor3f(1.0, 0.0, 0.1);
	}
	if (velocidad>= 7) {
		
		glColor3f(1, 0, 0.4);
	}
	glVertex3f(1 - (1.4 - 0.2) * velocidad/ (20 + 0.000002), 0.8, -2.0);
	if (velocidad>= 10) {
		glColor3f(0.7, 0.5, 0);
	}
	glVertex3f(1 - (1.4 - 0.2) * velocidad/ (20 + 0.000002), 0.8, -2.0);
	glEnd();
	glPopMatrix();
}




void display()
// Funcion de atencion al dibujo
{
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	
	//Coche y barra
	if (solidario == ACT) {			
		barra_Velocidad();
		glPushMatrix();
		//colocarlo odnde toca
		glTranslatef(0, -0.9, 0);
		coche();
		glPopMatrix();
	}


	if (noche_Dia == NOCHE) {		
		glPushMatrix();
		es_Noche();
		glPopMatrix();
	}
	else {
		glClearColor(1, 1, 1, 1);
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		glPopMatrix();
	}


	if (alambrico_solido == ALAMBRICO) {
		glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
	}

	if (niebla == ON) {	

		glPushMatrix();
		glEnable(GL_FOG);		
		GLfloat color_niebla[] = { 0.6 ,0.65,0.8 };
		glFogfv(GL_FOG_COLOR, color_niebla);		
		glFogf(GL_FOG_DENSITY, 0.1);		
		glPopMatrix();
	}
	else {
		glDisable(GL_FOG);
	}

	if (ceniza == ONl) {		
		
		
		glPushMatrix();	
		
		continuar_ceniza();
		
		glPopMatrix();
	
	}
	else {
		glPushMatrix();		
		glPopMatrix();


	}


	gluLookAt(distancia_X, distancia_Y, distancia_Z, distancia_X + giro_X, 1, distancia_Z + giro_Z, 0, 1, 0);

	
	farolas();
	pista();
	carteles1();
	fondos();	
	suelo();
	 // Posición de arranque
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, meta);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (noche_Dia == DIA) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); }	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-3, 0.01, -5);
	glTexCoord2f(1, 0);
	glVertex3f(3, 0.01, -5);
	glTexCoord2f(1, 1);
	glVertex3f(3, 0.01, -3);
	glTexCoord2f(0, 1);
	glVertex3f(-3, 0.01, -3);
	glEnd();
	glPopMatrix();
	//

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)

{
	glViewport(0, 0, w, h);
	float razon = (float)w / h;
	glMatrixMode(GL_PROJECTION);	
	// La primera posición marca grados FOVY
	glLoadIdentity();
	gluPerspective(FOVY, razon, 1, 1000);
}
void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case 's':
		if (alambrico_solido == ALAMBRICO) { alambrico_solido = SOLIDO; }
		else alambrico_solido = ALAMBRICO;
		break;

	case 'l':
		if (noche_Dia == NOCHE) { noche_Dia = DIA; }
		else noche_Dia = NOCHE;
		break;

	case 'n':
		if (niebla == ON) { niebla = OFF; }
		else niebla = ON;
		break;

	case 'c':
		if (solidario == ACT) { solidario = DES; }
		else solidario = ACT;
		break;

	case 'g':
		if (ceniza == ONl) { ceniza = OFFl; }
		else ceniza = ONl;
		break;

	case 27:
		exit(0);
	}
	glutPostRedisplay();
}


void update()
{
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempo_transcurrido = (ahora - antes) / 1000.0f;
	alfa +=  360 * tiempo_transcurrido;
	avanzado = tiempo_transcurrido * velocidad;	
	distancia_X += avanzado * giro_X;
	distancia_Z += avanzado * giro_Z;
	antes = ahora;
	glutPostRedisplay();
}

void onTimer(int tiempo){
	
	glutTimerFunc(tiempo, onTimer, tiempo);
	update();
}

void onArrow(int tecla, int x, int y)
{
	switch (tecla) {
	case GLUT_KEY_UP:
		
		velocidad += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (velocidad > 0.1) velocidad -= 0.1;
		else velocidad = 0.0000000;
		break;
	case GLUT_KEY_LEFT:
		//Vector director de la velocidad gira ¼ de grado respecto al eje Y
		angulo += 0.4;
		break;
	case GLUT_KEY_RIGHT:
		//Vector director de la velocidad gira ¼ de grado respecto al eje Y
		angulo -= 0.4;
		break;
	}
	giro_Z = cos(angulo * PI / 180);
	giro_X = sin(angulo * PI / 180);

	stringstream title;
	title << "Velocidad:   " << velocidad << "m/s";
	glutSetWindowTitle(title.str().c_str());

	glutPostRedisplay();
}





void main(int argc, char** argv)
{
	//FreeImage_Initialise();
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(WCX, WCY); 
	glutCreateWindow(PROYECTO); 
	init(); 
	std::cout << "CONDUCCION"  << std::endl; 

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);
	glutSpecialFunc(onArrow);
	glutKeyboardFunc(onKey);
	glutMainLoop(); 
	//FreeImage_DeInitialise();
}