#version 330

layout (location = 0) in vec3 iPosition;

uniform vec3 lineColor;
uniform mat4 gMVP;

out vec4 color;

void main() {
    gl_Position = gMVP * vec4(iPosition, 1.0);
    color = vec4(lineColor, 1.0);
}
