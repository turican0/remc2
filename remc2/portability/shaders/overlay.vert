// overlay.vert
#version 450

layout(location = 0) out vec2 outUV;

void main()
{
    // Fullscreen triangle: 3 vertices, no vertex buffer needed.
    vec2 pos = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    outUV = pos;
    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);
}