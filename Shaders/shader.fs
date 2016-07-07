#version 330

in vec4 fColor;
out vec4 FragColor;

void main()
{
    FragColor = fColor;
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
