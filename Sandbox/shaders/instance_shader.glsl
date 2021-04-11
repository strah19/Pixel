#shader vertex
#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 5) in float id;

layout(binding = 0) buffer GlobalMatrices 
{
    mat4 proj_view[];
};

out flat vec4 outColor;

void main()
{
	gl_Position = proj_view[int(id)] * vec4(aPos, 1.0);
	outColor = aColor;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in flat vec4 outColor;

uniform sampler2D ourTexture[32];

void main()
{
	FragColor = outColor;
}