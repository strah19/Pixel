#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float tex_index;

uniform mat4 proj;

out vec4 outColor;
out vec2 TexCoord;
out float index;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
	index = tex_index;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in vec4 outColor;
in vec2 TexCoord;
in float index;

uniform sampler2D ourTexture[32];

void main()
{
	int i = int(index);
	if(i > 0){
		FragColor = texture(ourTexture[i], TexCoord) * outColor;
	}
	else {
		FragColor = outColor;
	}
}