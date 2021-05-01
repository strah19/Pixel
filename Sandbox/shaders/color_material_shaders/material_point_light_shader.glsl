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
out flat float out_material_id;

void main()
{
	gl_Position = proj_view * vec4(pos, 1.0);
	out_color = color;
	out_tex_coord = tex_coord;
	out_tex_index = tex_index;
	out_pos = vec4(pos, 1.0);
    out_normal = normal;
    out_material_id = material_id;
}

#shader fragment
#version 450 core

out vec4 frag_color;

in flat vec4 out_color;
in vec2 out_tex_coord;
in flat float out_tex_index;
in vec4 out_pos;
in vec3 out_normal;
in flat float out_material_id;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 shininess;
};

struct Light {
    vec4 position;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};

layout(binding = 1) buffer Materials 
{
    Light light;
    Material material;
    vec4 view_pos;
};

uniform sampler2D textures[32];

void main()
{
    vec4 ambient  = light.ambient * material.ambient;

    vec4 norm = normalize(vec4(out_normal, 1.0));
    vec4 light_dir = normalize(light.position - out_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec4 diffuse  = light.diffuse * (diff * material.diffuse);
    
    vec4 view_dir = normalize(view_pos - out_pos);
    vec4 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess.x);
    vec4 specular = light.specular * (spec * material.specular);  
        
    float distance    = length(light.position - out_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));  

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation; 

    vec3 result = vec3(ambient + diffuse + specular);
    frag_color = vec4(result, 1.0);
}