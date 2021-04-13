#shader vertex
#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float tex_index;
layout(location = 4) in vec3 aNorm;
layout(location = 5) in float material_id;

layout(binding = 0) buffer GlobalMatrices 
{
    mat4 proj_view;
};

out flat vec4 outColor;
out vec2 TexCoord;
out flat float index;
out vec4 pos;

void main()
{
	gl_Position = proj_view * vec4(aPos, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
	index = tex_index;
	pos = vec4(aPos, 1.0);
}

#shader fragment
#version 450 core
out vec4 FragColor;
in flat vec4 outColor;
in vec2 TexCoord;
in flat float index;
in vec4 pos;

uniform sampler2D ourTexture[32];

float smoothedge(float v) {
    return smoothstep(0.0, 1.0 / 1280.0, v);
}

float roundRect(vec2 p, vec2 size, float radius) {
  vec2 d = abs(p) - size;
  return min(max(d.x, d.y), 0.0) + length(max(d,0.0))- radius;
}

void main()
{
	vec2 st = pos.xy / pos.w;
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
}