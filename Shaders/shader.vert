#version 330
layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;

out vec2 textureCoord;

uniform mat4 model;  
uniform mat4 view;  
uniform mat4 projection;

void main()
{
    gl_Position =  projection * view * model * vec4(inVertexPosition, 1.0);
    textureCoord = inTextureCoord;
}
