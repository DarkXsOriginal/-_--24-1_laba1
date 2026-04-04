#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : programID(0) {}

Shader::~Shader() {
    if (programID) {
        glDeleteProgram(programID);
    }
}

std::string loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot open file " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(const std::string& source, GLenum type, const std::string& typeName) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR: SHADER_COMPILATION_ERROR of type: " << typeName << "\n" << infoLog << "\n";
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = ::loadFromFile(vertexPath);
    std::string fragmentSource = ::loadFromFile(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty()) {
        return false;
    }

    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER, "VERTEX");
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    if (!vertexShader || !fragmentShader) {
        return false;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 1024, NULL, infoLog);
        std::cout << "ERROR: SHADER_PROGRAM_LINKING_FAILED\n" << infoLog << "\n";
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

Shader& Shader::use() {
    glUseProgram(programID);
    return *this;
}

void Shader::unuse() {
    glUseProgram(0);
}

GLuint Shader::getID() {
    return programID;
}

Shader& Shader::set(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    return *this;
}

Shader& Shader::set(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    return *this;
}

Shader& Shader::set(const std::string& name, const float* values, int count) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), count, values);
    return *this;
}