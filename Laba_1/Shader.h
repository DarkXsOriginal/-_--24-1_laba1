#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader {
private:
    GLuint programID;

public:
    Shader();
    ~Shader();

    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    Shader& use();
    void unuse();
    GLuint getID();

    Shader& set(const std::string& name, int value);
    Shader& set(const std::string& name, float value);
    Shader& set(const std::string& name, const float* values, int count);
};

#endif