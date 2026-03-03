// Laba_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

int main()
{


    glfwInit();

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3. \n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    /*glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/


    

    GLFWwindow* MyWindow;
    MyWindow = glfwCreateWindow(512, 512, "MyWindow", NULL, NULL);


    if (!MyWindow) {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(MyWindow);

    glewExperimental = GL_TRUE;

    glewInit();

    const GLubyte* version_str = glGetString(GL_VERSION);
    const GLubyte* device_str = glGetString(GL_RENDERER);

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("This version OpenGL running is %s\n", version_str);
    printf("This device OpenGL running is %s\n", device_str);


    while (!glfwWindowShouldClose(MyWindow)) {

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2, 1.0, 1.0, 1.0);

        glBegin(GL_POLYGON);
        glColor3f(1.0, 0.8, 0.5);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(-0.5, 0.5);
        glEnd();

        glfwSwapBuffers(MyWindow);
        glfwPollEvents();


    }

    

    glfwTerminate();

}

