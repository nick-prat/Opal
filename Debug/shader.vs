#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gMVP;

out vec4 Color;

void main()
{
	gl_Position = gMVP * vec4(Position, 1.0);
	Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
    //gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);
}
