#version 330 core

out vec4 FragColor;
in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;
vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}


float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return 0.5+0.5*(clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos, float near, float far) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);

    // Normalize clip space depth to the range [-1, 1]
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;

    // Calculate linear depth using perspective division
    float linearDepth = (near * far) / (far + near - clip_space_depth * (far - near));

    // Normalize linear depth between 0.0 and 1.0
    return linearDepth / far;
}


void main() {

    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint-nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D,0.1,100);
    float fading = max(0, (0.5 - linearDepth));

    FragColor = (grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true)) * float(t > 0);
    FragColor.a *= fading;
    if(FragColor.a==0.0)
    {
        discard;
    }
}
