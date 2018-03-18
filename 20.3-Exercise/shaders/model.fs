#version 330 core

#define NR_POINT_LIGHTS     4

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

struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform vec3 cameraPos;

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);

    // Combine results
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    // vec3 normalMap = normalize(texture( material.normal, TexCoords).rgb*2.0 - 1.0);
    //vec3 heightMap =  light.specular * spec * vec3(texture(material.height, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    // Diffuse Shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular Shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 1. Directional Ligthing
    vec3 result = CalcDirectionalLight(dirLight, norm, viewDir);

    // 2. Point light
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // 3. Spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, norm, ratio);
    vec4 refraction = texture(material.height, TexCoords) * texture(skybox, R);

    FragColor = vec4(result, 4.0) + refraction;
    //vec3 diffuse = texture(material.diffuse, TexCoords).rgb;
    //FragColor = texture(material.diffuse, TexCoords) + texture(material.specular, TexCoords) * texture(skybox, R);//vec4(texture(skybox, R).rgb, 1.0);
}