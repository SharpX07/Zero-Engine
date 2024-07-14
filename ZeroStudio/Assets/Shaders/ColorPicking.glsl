#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#type fragment
#version 330 core

//uniform uint EntityIDUniform;
//uniform bool Useu_EntityId;

void main()
{
    resultColor = vec4( 1.0f);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Color blanco por defecto
    EntityID = uint(u_EntityId);
}