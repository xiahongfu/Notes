#version 330 core
out vec4 FragColor;  

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissionMap;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 direction;
};
struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};
#define NR_POINT_LIGHTS 4

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;


vec3 CalcPointLight(PointLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord);
vec3 CalcDirectionalLight(DirectionalLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord);
vec3 CalcSpotLight(SpotLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord);
void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;
    for (int i = 0; i < NR_POINT_LIGHTS; i++) 
    {
        vec3 pointResult = CalcPointLight(pointLight[i], material, viewDir, FragPos, TexCoord) * objectColor;
        result = result + pointResult;
    }

    vec3 dirResult = CalcDirectionalLight(dirLight, material, viewDir, FragPos, TexCoord) * objectColor;
    result = result + dirResult;
    vec3 spotResult = CalcSpotLight(spotLight, material, viewDir, FragPos, TexCoord) * objectColor;
    result = result + spotResult;
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * light.color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * matDiffuse * diff * light.color;
    
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 matSpecular = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular * matSpecular * spec * light.color;

    float distance = length(light.position - FragPos);
    vec3 lightResult = (ambient+diffuse+specular)/(light.constant + distance * light.linear + distance * distance * light.quadratic);
    return lightResult;
}

vec3 CalcDirectionalLight(DirectionalLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * light.color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * matDiffuse * diff  * light.color;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 matSpecular = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular * matSpecular * spec * light.color;

    vec3 lightResult = (ambient+diffuse+specular);
    return lightResult;
}
vec3 CalcSpotLight(SpotLight light, Material material, vec3 viewDir, vec3 FragPos, vec2 TexCoord)
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightTargetDir = normalize(-light.direction);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * light.color;
    float theta = dot(lightDir, lightTargetDir);
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * matDiffuse * diff  * light.color * intensity;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 matSpecular = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular * matSpecular * spec * light.color * intensity;

    vec3 lightResult = (ambient+diffuse+specular);
    return lightResult;
}