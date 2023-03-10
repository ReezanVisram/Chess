#version 460 core
struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;
uniform bool isSelected;
uniform bool isLegal;

void main() {
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.diffuse, TexCoord).rgb;

	vec3 result = ambient + diffuse + specular;

	if (isSelected) {
		FragColor = vec4(result, 1.0) * vec4(1.0, 0.0, 0.0, 1.0);
	} else if (isLegal) {
		FragColor = vec4(result, 1.0) * vec4(0.0, 1.0, 0.0, 1.0);
	} else {
		FragColor = vec4(result, 1.0);
	}
}