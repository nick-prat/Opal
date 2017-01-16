#version 330

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D gSampler;
uniform vec3 gAmbientColor;
uniform float gAmbientIntensity;

void main() {
    FragColor = texture2D(gSampler, texCoord) * vec4(gAmbientColor.xyz, 1.0f) * gAmbientIntensity;
}
