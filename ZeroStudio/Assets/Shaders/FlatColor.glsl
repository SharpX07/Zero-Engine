#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 o_Fragment;

in vec3 FragPos;

void main()
{    
    o_Fragment = vec4(0.0,1.0,1.0,1.0);
}