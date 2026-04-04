#version 460 core
layout (location = 0) in vec2 vp;

void main() {
    gl_Position = vec4(vp.x, vp.y, 0.0, 1.0);
}