#define GLEW_DLL
#define GLFW_DLL

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float fov = 45.0f;

float vertex[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
};

void processInput(GLFWwindow* window) {
    float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * 0.1f;
    float yoffset = (lastY - ypos) * 0.1f;
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* MyWindow;
    MyWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyWindow", NULL, NULL);

    if (!MyWindow) {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(MyWindow);
    glfwSetCursorPosCallback(MyWindow, mouse_callback);
    glfwSetInputMode(MyWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "ERROR: could not initialize GLEW: %s\n", glewGetErrorString(err));
        return -3;
    }

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
        processInput(MyWindow);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        glm::mat4 transform = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        shaderProgram.use().setMat4("model", glm::value_ptr(model));
        shaderProgram.use().setMat4("view", glm::value_ptr(view));
        shaderProgram.use().setMat4("projection", glm::value_ptr(projection));
        shaderProgram.use().setMat4("transform", glm::value_ptr(transform));

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

