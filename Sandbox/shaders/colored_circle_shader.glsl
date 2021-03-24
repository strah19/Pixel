#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float tex_index;

uniform mat4 proj_view;

out flat vec4 outColor;
out vec2 TexCoord;
out flat float index;

void main()
{
	gl_Position = proj_view * vec4(aPos, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
	index = tex_index;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in flat vec4 outColor;
in vec2 TexCoord;
in flat float index;

uniform sampler2D ourTexture[32];

void main()
{
	if(index != -1.0){
		if(outColor == vec4(-1, -1, -1, -1)){
			FragColor = texture(ourTexture[int(index)], TexCoord);
		}
		else{
			FragColor = texture(ourTexture[int(index)], TexCoord) * outColor;
		}
	}
	else {
		FragColor = outColor;
	}
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}