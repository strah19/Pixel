#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 outColor;
out vec2 TexCoord;

uniform float offset;

void main()
{
gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in vec3 outColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}