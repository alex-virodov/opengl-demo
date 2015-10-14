// std headers
#include <iostream>
#include <memory>

// windows/gl headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// local headers
#include "GLFW/glfw3.h"
#include "exception.h"
#include "irange.h"

// libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw3.lib")    // for now using static version

// Some unique-ptr helpers
template <class T>
using uptr = std::unique_ptr<T>;

template <class T>
uptr<T> make_uptr(T*&& ptr) { return uptr<T>(ptr); }




using namespace std;

// too bad GLFW doesn't support lambdas...
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE) 
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Using fixed pipeline for its simplicity
	glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawGrid()
{
    static const float size = 10;
    
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for (float x = -size; x <= size; x += 1.0f)
    {
        glVertex3f(x, 0, -size);
        glVertex3f(x, 0, +size);
        
        glVertex3f(-size, 0, x);
        glVertex3f(+size, 0, x);
    }
    glEnd();
}

void drawTriangleSquare()
{
        glTranslatef(-1.5f,0.0f,0.0f);						// Move Left 1.5 Units And Into The Screen 6.0
//	    glRotatef(rtri,0.0f,1.0f,0.0f);						// Rotate The Triangle On The Y axis ( NEW )
	    glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
		    glColor3f(1.0f,0.0f,0.0f);						// Set Top Point Of Triangle To Red
		    glVertex3f( 0.0f, 1.0f, 0.0f);					// First Point Of The Triangle
		    glColor3f(0.0f,1.0f,0.0f);						// Set Left Point Of Triangle To Green
		    glVertex3f(-1.0f,-1.0f, 0.0f);					// Second Point Of The Triangle
		    glColor3f(0.0f,0.0f,1.0f);						// Set Right Point Of Triangle To Blue
		    glVertex3f( 1.0f,-1.0f, 0.0f);					// Third Point Of The Triangle
	    glEnd();											// Done Drawing The Triangle
	    glTranslatef(3.0f,0.0f,0.0f);						// Move Right 1.5 Units And Into The Screen 6.0
//	    glRotatef(rquad,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
	    glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
	    glBegin(GL_QUADS);									// Draw A Quad
		    glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		    glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		    glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		    glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	    glEnd();											// Done Drawing The Quad

}



int main()
{
    cout << "begin" << endl;
    
    check_state_msg(glfwInit(), "GL init failed");

    glfwWindowHint(GLFW_RESIZABLE, FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(640, 480, "hello", NULL, NULL);
    
    check_state_msg(window != NULL, "window creation failed");

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // initial viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

	glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glLoadIdentity();
	    glTranslatef(0.0f,-1.0f,-12.0f);
        drawGrid();

        drawTriangleSquare();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}