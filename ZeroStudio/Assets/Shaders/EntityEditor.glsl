#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{
	FragPos = vec3(model*vec4(aPos, 1.0));
	gl_Position = projection * view * model* vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	mat3 normalMatrix = transpose(inverse(mat3(model)));

    // Transformación de la normal del vértice
    Normal = normalize(normalMatrix * aNormal);
}

#type fragment

#version 330 core
layout(location = 0) out vec4 o_Fragment;     // Color normal

layout(location = 1) out uint o_EntityID;      // ID de la entidad para picking

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;

// Nuevas propiedades del material
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform bool hasAlbedoTexture; // Nuevo uniforme

// Propiedades de la luz
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform uint u_EntityId;

// Uniform para indicar si el objeto está marcado
uniform bool isMarked;

void main()
{
    vec3 baseColor;
    float transparency = 0.0;
    if (hasAlbedoTexture) {
        // Combinar albedo con la textura
        vec4 texColor = texture(ourTexture, TexCoord).rgba;
        transparency = texColor.a;
        baseColor = albedo * texColor.rgb;
    } else {
        baseColor = albedo; // Usar solo el color de albedo
        transparency = 1.0;
    }

    // Efecto de puntos si el objeto está marcado
    if (isMarked) {
        float pattern = step(0.5, mod(floor(TexCoord.x * 10.0) + floor(TexCoord.y * 10.0), 2.0));
        baseColor = mix(baseColor, Normal, pattern); // Alternar entre el color base y rojo
    }

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * baseColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;

    // specular
    float specularStrength = 1.0 - roughness; // Usar roughness para controlar la intensidad especular
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * mix(vec3(1.0), baseColor, metallic); // Usar metallic para mezclar con el color base

    vec3 result = ambient + diffuse + specular;
    o_Fragment = vec4(result, 1.0);
    o_EntityID = uint(u_EntityId)+1u;
    if (transparency == 0.0) {
        discard;
    }
}
