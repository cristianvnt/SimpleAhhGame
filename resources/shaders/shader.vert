#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 color;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(aPos, 1.f);
	color = vec4(1.0f, 0.4f, 0.3f, 1.0f);
}
