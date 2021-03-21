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
out vec2 pos;

void main()
{
	gl_Position = proj_view * vec4(aPos, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
	index = tex_index;
    pos = aPos.xy;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in flat vec4 outColor;
in vec2 TexCoord;
in flat float index;
in vec2 pos;

uniform sampler2D ourTexture[32];

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
}

void main()
{
 	vec2 st = pos;
    float y = st.x;

    vec3 color = vec3(circle(st, 0.9)) - (vec3(1.0) - outColor.xyz);

    float a = 1.0;
    if(color != outColor.xyz)
        a = 0.0;
	gl_FragColor = vec4(color, a);
}