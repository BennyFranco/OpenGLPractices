#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
     // Otain normal from normal map in range [0,1]
    vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
    // Normalize
    normal = normalize(normal * 2.0 - 1.0);

    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 ambient = 0.1 * color;

    // Diffuse
    
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diff * color;

    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflecDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir +  viewDir);
    float spec = pow(max(dot(normal, halfwayDir),0.0), 32.0);

    vec3 specular = vec3(0.2) * spec * vec3(texture(material.specular, fs_in.TexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}