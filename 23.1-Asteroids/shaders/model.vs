#version 330 core
layout (location = 0) in vec3 aPos;		// The position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() 
{ 
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}