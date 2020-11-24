#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D texture1;

void main()
{
	vec2 texCoord = textureCoord * vec2(1.0, -1.0);
	FragColor = texture(texture1,texCoord);
}

