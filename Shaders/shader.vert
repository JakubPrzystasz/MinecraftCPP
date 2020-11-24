#version 330
layout(location = 0) in vec3 inVertexPosition;
layout(location = 2) in vec2 inTextureCoord;

out vec2 textureCoord;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = vec4(inVertexPosition, 1.0);

    textureCoord = inTextureCoord;
}
