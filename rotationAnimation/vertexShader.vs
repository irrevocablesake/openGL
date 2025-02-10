#version 460 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexture;
layout ( location = 2 ) in vec3 aColor;
out vec2 outTextureCoord;
out vec3 outColor;

uniform mat4 transformationMatrix;

void main()
{
    gl_Position = transformationMatrix * vec4( aPos, 1.0);
    outTextureCoord = aTexture;
    outColor = aColor;
}