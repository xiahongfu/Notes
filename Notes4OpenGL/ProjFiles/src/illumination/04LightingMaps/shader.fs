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

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * light.color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 tmp = vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * tmp * diff  * light.color;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 tmp2 = vec3(1.0) - vec3(texture(material.specular, TexCoord));  // 反转镜面光贴图的颜色值 
    vec3 tmp2 = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular * tmp2 * spec * light.color;

    vec3 emission = vec3(texture(material.emissionMap, TexCoord));
    vec3 result = (ambient+diffuse+specular+emission) * objectColor;
    FragColor = vec4(result, 1.0);
}