#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

using namespace std;

float xTriangulo = 0.0f;
float yTriangulo = 0.0f;

float velocidad = 0.7f;
float velocidadActual = 0.0f;
float aceleracion = 0.8f;
float desaceleracion = 0.4f;

float xCuadrado = -0.35f;
float yCuadrado = -0.4f;

float angulorotacionTriangulo = 0.0f;
float anguloTrayectoria = 0.0f;
float velocidadAngular = 90.0f;

// Creamos una ventana
GLFWwindow *window;

double tiempoAnterior = 0.0;
double tiempoActual = 0.0;
double tiempoDiferencial = 0.0;

float rTriangulo = 1.0f;
float gTriangulo = 1.0f;
float bTriangulo = 1.0f;

void dibujar();
void dibujarTriangulo();
void dibujarCuadrado();
void actualizar();
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moverTriangulo();
void checarColisiones();

int main()
{
	// Si no se puede inicializar glfw, finalizamos la ejecucion
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Si se pudo iniciar GLFW, entonces inicializamos la ventana
	window = glfwCreateWindow(1024, 768, "Ventana", NULL, NULL);
	
	// Si no podemos iniciar la ventana, terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	// Una vez establecido el contexto, activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL << std::endl;

	glfwSetKeyCallback(window, teclado_callback);

	tiempoAnterior = glfwGetTime();

	//Ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		
		//Establecer region de dibujo
		glViewport(0, 0, 1024, 768);

		//Establece el color de borrado
		glClearColor(.1, 0.6, 0.8, 1);

		//Borramos
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();
		
		//Cambiar los buffers
		glfwSwapBuffers(window);

		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

void dibujar()
{
	dibujarCuadrado();
	dibujarTriangulo();
}

void actualizar()
{
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;

	checarColisiones();
	moverTriangulo();

	tiempoAnterior = tiempoActual;
}

void dibujarTriangulo()
{
	glPushMatrix();

		//Transformaciones
		glTranslatef(xTriangulo, yTriangulo, 0);
		glRotatef(angulorotacionTriangulo, 0.0f, 0.0f, 1.0f);
		// glScalef(0,0,0);

		glBegin(GL_TRIANGLES);
			glColor3f(rTriangulo, gTriangulo, bTriangulo);
			glVertex3f(0.0f, 0.08f, 0.0f);
			glVertex3f(0.05f, -0.08f, 0.0f);
			glVertex3f(-0.05f, -0.08f, 0.0f);
		glEnd();
	glPopMatrix();
}

void dibujarCuadrado()
{
	glPushMatrix();
		glTranslatef(xCuadrado, yCuadrado, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.4f, 0.5f);
			glVertex3f(-0.1f, 0.1f, 0.0f);
			glVertex3f(-0.1f, -0.1f, 0.0f);
			glVertex3f(0.1f, -0.1f, 0.0f);
			glVertex3f(0.1f, 0.1f, 0.0f);
		glEnd();
	glPopMatrix();
}

void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	/*if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		xTriangulo += velocidad;
	else

	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		xTriangulo -= velocidad;
	else

	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
		yTriangulo += velocidad;
	else

	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
		yTriangulo -= velocidad;*/

}

void moverTriangulo()
{
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (estadoTeclaDerecha == GLFW_PRESS || estadoTeclaDerecha == GLFW_REPEAT)
	{
		if (angulorotacionTriangulo < -360)
			angulorotacionTriangulo += 360;
		else
			angulorotacionTriangulo -= velocidadAngular * (float)tiempoDiferencial;
		// xTriangulo += velocidad * (float)tiempoDiferencial;
	}

	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoTeclaIzquierda == GLFW_PRESS)
	{
		if (angulorotacionTriangulo > 360)
			angulorotacionTriangulo -= 360;
		else
			angulorotacionTriangulo += velocidadAngular * (float)tiempoDiferencial;
		// xTriangulo -= velocidad * (float)tiempoDiferencial;
	}

	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	if (estadoTeclaArriba == GLFW_PRESS)
	{
		anguloTrayectoria = angulorotacionTriangulo;
		if (velocidadActual <= velocidad)
		{
			velocidadActual += aceleracion * tiempoDiferencial;
		}
		// yTriangulo += velocidad * (float)tiempoDiferencial;
	}
	
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);
	if (estadoTeclaAbajo == GLFW_PRESS)
	{
		anguloTrayectoria = angulorotacionTriangulo;
		if (velocidadActual <= velocidad)
		{
			velocidadActual -= desaceleracion * tiempoDiferencial;
		}
		// yTriangulo -= velocidad * (float)tiempoDiferencial;
	}

	if (!(estadoTeclaArriba == GLFW_PRESS && estadoTeclaAbajo == GLFW_PRESS))
	{
		if (velocidadActual > 0)
		{
			velocidadActual -= desaceleracion * tiempoDiferencial;
		}
		if (velocidadActual < 0)
		{
			velocidadActual += desaceleracion * tiempoDiferencial;
		}
	}

	xTriangulo += cos((anguloTrayectoria + 90.0f) * (3.14159f / 180.0f)) * velocidadActual * tiempoDiferencial;
	yTriangulo += sin((anguloTrayectoria + 90.0f) * (3.14159f / 180.0f)) * velocidadActual * tiempoDiferencial;

	// Teletransporte de la x
	if (xTriangulo < -1.2f)
	{
		xTriangulo = 1;
	}
	else
		if (xTriangulo > 1.1f)
		{
			xTriangulo = -1;
		}

	// Teletransporte de la y
	if (yTriangulo < -1.1f)
	{
		yTriangulo = 1;
	}
	else
		if (yTriangulo > 1.1f)
		{
			yTriangulo = -1;
		}
}

void checarColisiones()
{
	// Si el triángulo colisiona con el cuadrado
	if ((xTriangulo - 0.05f < xCuadrado + 0.1f) && (xTriangulo + 0.05f > xCuadrado - 0.1f) && (yTriangulo - 0.08f < yCuadrado + 0.1f) && (yTriangulo + 0.08f > yCuadrado - 0.1f))
	{
		// std::cout << "Colision" << endl;
		rTriangulo = 0.0f;
		gTriangulo = 0.0f;
		bTriangulo = 0.0f;
	}
	else
	{
		rTriangulo = 1.0f;
		gTriangulo = 1.0f;
		bTriangulo = 1.0f; 
	}
}