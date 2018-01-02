#version 330 core
layout (location = 0) in vec3 aPos;		// The position variable has attribute position 0
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;

void main() 
{ 
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;

	// This calculate the Normal vector from model
	// Normal = mat3(transpose(inverse(model))) * aNormal; 

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = projection * view * vec4(aPos, 1.0);
}