#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 gMVP;

out vec4 color;

void main()
{
    color = vec4(clamp(Normal, 0, 1), 1.0);
    gl_Position = gMVP * vec4(Position, 1.0);
}
