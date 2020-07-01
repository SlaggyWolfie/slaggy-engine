#version 330 core

out vec4 fragmentColor;  
in vec2 textureCoordinate;
uniform sampler2D textureSampler;

void main()
{
	fragmentColor = texture(textureSampler, textureCoordinate);
} 