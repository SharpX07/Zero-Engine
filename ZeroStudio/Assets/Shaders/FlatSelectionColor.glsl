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

uniform float u_Time;

void main()
{
    // Crear una cuadr�cula base
    vec2 uv = FragPos.xy * 10.0; // Ajusta el 10.0 para cambiar el tama�o de la cuadr�cula
    vec2 gv = fract(uv) - 0.5;
    
    // Crear un patr�n de onda
    float wave = sin(FragPos.x * 5.0 + u_Time) * cos(FragPos.y * 5.0 + u_Time) * 0.25;
    
    // Calcular la distancia al centro de cada celda
    float d = length(gv);
    
    // Crear un c�rculo en cada celda
    float circle = smoothstep(0.4 + wave, 0.4 + wave + 0.01, d);
    
    // Generar colores basados en la posici�n
    vec4 color = 0.5 + vec4(0.5 * cos(u_Time + uv.xyx + vec3(0,2,4)),1.0);
    
    // Combinar el c�rculo con el color
    color = mix(color, vec4(1.0,1.0,1.0,0.0), circle);
    
    // A�adir un borde a la cuadr�cula
    vec2 grid = abs(fract(uv - 0.5) - 0.5) / fwidth(uv);
    float line = min(grid.x, grid.y);
    color *= smoothstep(0.0, 0.1, line);
    
    o_Fragment = vec4(color);
    if (color.w==0.)
        discard;
}