#version 460 core
layout (location = 0) in vec2 vp;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * transform * vec4(vp.x, vp.y, 0.0, 1.0);
}