// std headers
#include <iostream>
#include <memory>
#include <list>

// local headers
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "exception.h"
#include "irange.h"
#include "gltexture.h"
#include "particle_generator.h"

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

void draw_grid()
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

void draw_marble_table(const GLTexture& marble)
{
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    marble.bind();

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    const int   sides  = 64;
    const float radius = 3.0f;

    for (int i : irange(sides+1))
    {
        double theta = (2*3.14/double(sides)) * i;

        glTexCoord2d(0.5 + sin(theta)/2.0, 0.5 + cos(theta)/2.0);
        glVertex3d(radius * sin(theta), 0.0, radius * cos(theta));
    }
    
    glEnd();

    glPopAttrib();
}

// This is simpler than implementing reading, and more self-contained.
// Could do resources, but overhead is same, and this is simpler still.
extern "C" const unsigned short image_marble_rgb565[];




int main()
{
    cout << "begin" << endl;
    
    check_state_msg(glfwInit(), "GL init failed");

    glfwWindowHint(GLFW_RESIZABLE, 0);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "hello", NULL, NULL);
    
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

    GLTexture marble = GLTexture(894, 894, (void*)image_marble_rgb565, GLTexture::Format::RGB565);

    ParticleGenerator generator;

    glfwSetTime(0.0);

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	    glLoadIdentity();
	    // glTranslatef(0.0f,-1.0f,-12.0f);

        const double camera_dist  = 10.0f;
        const double camera_angle = glfwGetTime() / 5.0;

        gluLookAt(
            /*eyex=*/camera_dist*sin(camera_angle), 
            /*eyey=*/3.0f, 
            /*eyez=*/camera_dist*cos(camera_angle), 
            /*center=*/0.0f, 1.0f, 0.0f, 
            /*up    =*/0.0f, 1.0f, 0.0f);


        glTranslatef(0.0f,-0.1f,0.0f);
        draw_grid();
        glTranslatef(0.0f,+0.1f,0.0f);

        glEnable(GL_STENCIL_TEST);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);

        draw_marble_table(marble);

        float mtx_invy[16] = {
            1.0f,   0.0f,   0.0f,   0.0f,
            0.0f,  -1.0f,   0.0f,   0.0f,
            0.0f,   0.0f,   1.0f,   0.0f,
            0.0f,   0.0f,   0.0f,   1.0f,
        };


        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF);

        // draw the "reflection" (since we know camera motion, it is easy)
        glDisable(GL_DEPTH_TEST);
        glMultMatrixf(mtx_invy);
        generator.draw(/*alpha_offset=*/1.0f);
        glMultMatrixf(mtx_invy);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);


        // draw the original
        generator.draw();
        
        
        generator.advance(0.01);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}