#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D blockTexture;

void main()
{
	vec4 texColor = texture(blockTexture, textureCoord * vec2(1.0, -1.0));
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}

