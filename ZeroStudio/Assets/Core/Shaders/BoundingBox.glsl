#type vertex

#version 330 core

// Grid position are in xy clipped space
uniform vec3 pos;
uniform mat4 projection;
uniform mat4 view;
out mat4 fragView;
out mat4 fragProj;
out vec3 nearPoint;
out vec3 farPoint;

vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);
// normal vertice projection
vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = gridPlane[gl_VertexID].xyz;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane
    fragView = view;
    fragProj = projection;
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}

#type fragment

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
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}


void main() {

    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint-nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D,0.1,100);
    float fading = max(0, (0.5 - linearDepth));

    FragColor = (grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true))* float(t > 0); // adding multiple resolution for the grid;
    FragColor.a *= fading;
    if(FragColor.a==0.0)
    {
        discard;
    }
}
