#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
out vec3 FragPos;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_BillboardPos;
uniform vec2 u_BillboardSize;
void main()
{
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexPosition_worldspace = 
        u_BillboardPos
        + cameraRight * aPos.x * u_BillboardSize.x
        + cameraUp * aPos.y * u_BillboardSize.y;

    FragPos = vertexPosition_worldspace;
    gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0);
    TexCoord = aTexCoord;
}

#type fragment

#version 330 core
layout(location = 0) out vec4 o_Fragment;     // Color normal
layout(location = 1) out uint o_EntityID;      // ID de la entidad para picking
in vec2 TexCoord;
in vec3 FragPos;
uniform uint u_EntityId;

uniform sampler2D ourTexture;

void main()
{
	vec4 texColor = texture(ourTexture, TexCoord).rgba;
    o_Fragment = texColor;
    o_EntityID = u_EntityId + 1u;
}
