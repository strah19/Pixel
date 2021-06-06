#shader vertex
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3, component = 0) in float tex_index;
layout(location = 3, component = 1) in float material_id;
layout(location = 4) in vec3 normal;

layout(std430, binding = 0) buffer GlobalMatrices 
{
    mat4 proj_view;
};

out flat vec4 out_color;
out vec2 out_tex_coord;
out flat float out_tex_index;
out vec4 out_pos;
out vec3 out_normal;

void main()
{
	gl_Position = proj_view * vec4(pos, 1.0);
	out_color = color;
	out_tex_coord = tex_coord;
	out_tex_index = tex_index;
	out_pos = vec4(pos, 1.0);
    out_normal = normal;
}

#shader fragment
#version 450 core

out vec4 frag_color;

in flat vec4 out_color;
in vec2 out_tex_coord;
in flat float out_tex_index;
in vec4 out_pos;
in vec3 out_normal;

uniform sampler2D textures[32];

void main()
{
	float dis = length(out_tex_coord);
	frag_color = vec4(out_color.rgb, out_color.a * (pow(0.01, dis) - 0.01));
}