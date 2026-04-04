#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

float vertex[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
};

int main()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* MyWindow;
    MyWindow = glfwCreateWindow(512, 512, "MyWindow", NULL, NULL);

    if (!MyWindow) {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(MyWindow);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "ERROR: could not initialize GLEW: %s\n", glewGetErrorString(err));
        return -3;
    }

    const GLubyte* version_str = glGetString(GL_VERSION);
    const GLubyte* device_str = glGetString(GL_RENDERER);

    printf("OpenGL version: %s\n", version_str);
    printf("OpenGL device: %s\n", device_str);

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("Context version: %d.%d\n", major, minor);

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader shaderProgram;
    if (!shaderProgram.loadFromFile("vertex.glsl", "fragment.glsl")) {
        std::cout << "ERROR: Failed to load shader program" << std::endl;
        return -4;
    }

    while (!glfwWindowShouldClose(MyWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 1.0f, 1.0f, 1.0f);

        float color[] = { 0.41f, 0.25f, 0.67f };
        shaderProgram.use().set("u_color", color, 1);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(MyWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}