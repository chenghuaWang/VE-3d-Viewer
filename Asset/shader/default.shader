##shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 cpu_ViewProjection;
uniform mat4 cpu_Transform;

out vec3 Normal;
out vec3 FragPos;

void main() {
	FragPos = vec3(cpu_Transform * vec4(a_Position, 1.0));
	Normal = a_Normal;
	gl_Position = cpu_ViewProjection * cpu_Transform * vec4(a_Position, 1.0);
}

##shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

void main() {
	vec3 lightPos = vec3(0.1, 0.1, 0.9);
	vec3 lightColor = vec3(0.2, 0.5, 0.5);
	vec3 objectColor = vec3(1.0, 0.6, 0.8);
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}