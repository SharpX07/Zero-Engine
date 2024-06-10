#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main() {
     // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1,1,1);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(10,0,10) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1,1,1);
            
    vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1.0)*texture(ourTexture, TexCoord).rgba;
}

