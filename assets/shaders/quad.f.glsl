#version 450 core

in vec2 Uv_fs;
in vec3 FragPos_fs;
in vec3 Normal_fs;

out vec4 Colour;

layout(location=5) uniform sampler2D tex;
layout(location=6) uniform vec3 lightPos;

void main()
{
	vec4 img = texture(tex, Uv_fs);

	//vec4 ambient = 1.0 * texture(tex, Uv_fs);

	vec3 norm = normalize(Normal_fs);
	vec3 lightDir = normalize(lightPos-FragPos_fs);
	float diff = max(dot(norm, lightDir), 0.0);
	diff *= 2;
	vec3 diffuse = diff * vec3(1, 1, 1);

	vec4 result = vec4(diffuse, 1.0f) * img;

	Colour = result;

	//bool override = lightPos.x == 0;
//
	//if (override)
	//	Colour = vec4(0, 0, 1, 1);
}
