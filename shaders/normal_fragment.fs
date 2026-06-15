#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 PixelPosition;

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 4

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha_value;
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

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cut_off;
    float outer_cut_off;
};


uniform bool useTexture;
uniform vec3 color;
uniform vec3 view_pos;
uniform Material material;

uniform int num_point_lights;
uniform int num_spot_lights;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];


uniform sampler2D ourTexture;

vec3 get_directional_light(vec3 norm, DirectionalLight in_light, Material in_material)
{
	// Ambiente
	vec3 ambient = in_light.ambient * in_material.ambient;

	// Difusa
	vec3 light_dir = normalize(-in_light.direction);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = in_light.diffuse * (diff * in_material.diffuse);

	// Specular
	vec3 view_dir = normalize(view_pos - PixelPosition);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), in_material.shininess);
	vec3 specular = in_light.specular * (in_material.specular * spec);

	return (ambient + diffuse + specular);// * color;
}

vec3 get_point_light(vec3 norm, PointLight in_light, Material in_material)
{
	// Ambiente
	vec3 ambient = in_light.ambient * in_material.ambient;

	// Difusa
	vec3 light_dir = normalize(in_light.position - PixelPosition);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = in_light.diffuse * (diff * in_material.diffuse);

	// Specular
	vec3 view_dir = normalize(view_pos - PixelPosition);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), in_material.shininess);
	vec3 specular = in_light.specular * (in_material.specular * spec);
	
	float distance = length(in_light.position - PixelPosition);
	float attenuation = 1.0 / (in_light.constant + in_light.linear * distance + in_light.quadratic * (distance * distance));
	
	return (ambient + diffuse + specular) * attenuation;// * color;
}

vec3 get_spotlight(vec3 norm, SpotLight in_light, Material in_material)
{
	vec3 light_dir = normalize(in_light.position - PixelPosition);
	
	float theta = dot(light_dir, normalize(-in_light.direction));
	float epsilon = in_light.cut_off - in_light.outer_cut_off;
	float intensity = clamp((theta - in_light.outer_cut_off) / epsilon, 0.0, 1.0);  

	
	// Ambiente
	vec3 ambient = in_light.ambient * in_material.ambient;

	// Difusa
	
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = in_light.diffuse * (diff * in_material.diffuse);
	diffuse  *= intensity;

	// Specular
	vec3 view_dir = normalize(view_pos - PixelPosition);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), in_material.shininess);
	vec3 specular = in_light.specular * (in_material.specular * spec);
	specular *= intensity;

	return (ambient + diffuse + specular);// * color;
	
}

void main()
{
    vec3 norm = normalize(Normal);

    vec3 result = get_directional_light(norm, directional_light, material);
    
    for (int i = 0; i < num_point_lights; i++)
        result += get_point_light(norm, point_lights[i], material);
    
    for (int i = 0; i < num_spot_lights; i++)
        result += get_spotlight(norm, spot_lights[i], material);

    //FragColor = vec4(result, material.alpha_value);


    if(useTexture)
    {
        vec4 tex_color = texture(ourTexture, TexCoord);
        FragColor = vec4(result * tex_color.rgb, tex_color.a);
        //FragColor = texture(ourTexture, TexCoord);
    }
    else
        FragColor = vec4(result, material.alpha_value);
}