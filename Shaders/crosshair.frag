#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D corssHairTexture;

void main()
{
	vec4 texColor = texture(corssHairTexture, textureCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}

