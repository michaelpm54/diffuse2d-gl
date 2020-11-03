#version 450 core

layout(location=0) in vec3 pos;
layout(location=1) in vec2 Uv;
layout(location=2) in vec3 Normal;

out vec2 Uv_fs;
out vec3 FragPos_fs;
out vec3 Normal_fs;

layout(location=3) uniform mat4 model;
layout(location=4) uniform mat4 camera;

void main()
{
	vec4 canonicalPos = model * vec4(pos, 1);
	gl_Position = camera * canonicalPos;

	Uv_fs = Uv;
	FragPos_fs = vec3(model * vec4(pos.x, pos.y, 0, 1));
	Normal_fs = Normal;
}
