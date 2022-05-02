##shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 cpu_ViewProjection;
uniform mat4 cpu_Transform;

void main() {
	gl_Position = cpu_ViewProjection * cpu_Transform * vec4(a_Position, 1.0);
}

##shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = vec4(1.0);
}