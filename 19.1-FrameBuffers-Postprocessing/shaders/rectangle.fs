#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

// For kernel effects (Sharpen)
const float offset = 1.0 / 300.0;  

void main()
{ 
    // Normal
    FragColor = vec4(0,0.2,1.0,0.2);
}