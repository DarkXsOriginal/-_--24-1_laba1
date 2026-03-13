#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Важно для Mac
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Core profile

 
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


    const char* vert_shader =
        "#version 460 core\n" 
        "layout (location = 0) in vec2 vp;"  
        "void main() {"
        "  gl_Position = vec4(vp.x, vp.y, 0.0, 1.0);"
        "}";

    const char* frag_shader =
        "#version 460 core\n"  
        "out vec4 ourColor;"
        "void main() {"
        "  ourColor = vec4(0.41, 0.25, 0.67, 1.0);"
        "}";

   
    GLuint vertS = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragS = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertS, 1, &vert_shader, NULL);
    glShaderSource(fragS, 1, &frag_shader, NULL);

    glCompileShader(vertS);


    int success;
    char infoLog[512];
    glGetShaderiv(vertS, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertS, 512, NULL, infoLog);
        std::cout << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    glCompileShader(fragS);


    glGetShaderiv(fragS, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragS, 512, NULL, infoLog);
        std::cout << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertS);
    glAttachShader(shader_program, fragS);
    glLinkProgram(shader_program);


    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR: Shader program linking failed\n"  << infoLog << std::endl;
    }


    glDeleteShader(vertS);
    glDeleteShader(fragS);


    while (!glfwWindowShouldClose(MyWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 1.0f, 1.0f, 1.0f);

        glUseProgram(shader_program);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  

        glBindVertexArray(0);

        glfwSwapBuffers(MyWindow);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}