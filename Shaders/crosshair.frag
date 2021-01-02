#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D texture1;

void main()
{
	vec4 texColor = texture(texture1, textureCoord * vec2(1.0, -1.0));
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}

