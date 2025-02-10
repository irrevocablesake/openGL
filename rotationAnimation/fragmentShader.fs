#version 460 core
out vec4 fragColor;
in vec2 outTextureCoord;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
in vec3 outColor;
void main()
{
    fragColor = mix(texture( ourTexture, outTextureCoord), texture( ourTexture2, outTextureCoord ), 0.5) * vec4( outColor, 1.0);
}