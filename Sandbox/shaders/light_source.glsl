#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float tex_index;
layout(location = 4) in vec3 aNormal;

uniform mat4 proj_view;

out flat vec4 outColor;
out vec2 TexCoord;
out flat float index;
out vec3 FragPos;  
out vec3 Normal;  

void main()
{
	gl_Position = proj_view * vec4(aPos, 1.0);
	outColor = aColor;
	TexCoord = aTexCoord;
	index = tex_index;
	FragPos = aPos;
	Normal = aNormal;
}

#shader fragment
#version 450 core
out vec4 FragColor;
in flat vec4 outColor;
in vec2 TexCoord;
in flat float index;
in vec3 Normal;  
in vec3 FragPos;  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

uniform sampler2D ourTexture[32];
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;
uniform Material material;

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

    vec3 ambient  = light.ambient * material.ambient;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor *= vec4(result, 1.0);
}