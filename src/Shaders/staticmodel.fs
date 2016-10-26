#version 330

in vec4 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    //FragColor = color;
    FragColor = mix(texture2D(gSampler, texCoord), color, 0.5);
}
