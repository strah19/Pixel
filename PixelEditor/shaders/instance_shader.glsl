#shader vertex
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in float texture_id;
layout (location = 3) in vec4 color;
layout (location = 4) in mat4 model;

layout(binding = 0) buffer GlobalMatrices 
{
    mat4 proj_view;
};

out vec4 out_color;
out flat float out_texture_id;
out vec2 out_tex_coords;

void main()
{
	gl_Position = proj_view * model * vec4(pos, 1.0);
	out_color = color;
	out_texture_id = texture_id;
	out_tex_coords = tex_coords;
}

#shader fragment
#version 450 core

in vec4 out_color;
in flat float out_texture_id;
in vec2 out_tex_coords;

out vec4 frag_color;

uniform sampler2D textures[32];

void main()
{
	frag_color = texture(textures[int(out_texture_id)], out_tex_coords);
}