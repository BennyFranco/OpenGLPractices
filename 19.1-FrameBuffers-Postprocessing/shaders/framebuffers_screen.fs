#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

// For kernel effects (Sharpen)
const float offset = 1.0 / 300.0;  

void main()
{ 
    // Normal
    //FragColor = texture(screenTexture, TexCoords);

    // Inversion
    // FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 0);

    // Grayscale
    /* FragColor = texture(screenTexture, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0); */

    // Grayscale with weighted channels
    /* FragColor = texture(screenTexture, TexCoords);
    float average = (0.2126 * FragColor.r )+ (0.7152 * FragColor.g) + (0.0722 * FragColor.b);
    FragColor = vec4(average, average, average, 1.0); */

    // == Kernel Effects

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    // Sharpen
    /* float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    ); */

    // Blur
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    ); 

    // Edge detection
    // float kernel[9] = float[](
    //     1,  1,  1,
    //     1, -8,  1,
    //     1,  1,  1
    // );
    vec3 col = vec3(0.0);
    float vx_offset = 0.85;

    if (TexCoords.t>=(vx_offset +0.01))
    {
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];
    }
    else if (TexCoords.t<(vx_offset-0.01))
    {
        col = texture(screenTexture, TexCoords.st).rgb;
    }

    FragColor = vec4(col, 1.0);
}