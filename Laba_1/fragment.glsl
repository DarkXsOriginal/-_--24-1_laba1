#version 460 core
out vec4 ourColor;

uniform vec3 u_color;

void main() {
    ourColor = vec4(u_color, 1.0);
}