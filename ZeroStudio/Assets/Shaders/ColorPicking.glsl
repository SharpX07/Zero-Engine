#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{
	gl_Position = projection * view * model* vec4(aPos, 1.0f);
}

#type fragment

#version 330 core

layout (location = 0) out vec3 Vector;
out vec4 FragColor;

in vec3 FragPos;
uniform vec3 ColorPick;

void main()
{
    FragColor = vec4(ColorPick,1.0f);
}
