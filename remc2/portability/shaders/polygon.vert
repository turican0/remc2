#version 450

layout(location = 0) in vec3  inPosW;    // NDC x, y, and w
layout(location = 1) in vec2  inUV;      // texel coordinates, 0..32
layout(location = 2) in float inShade;   // 0..1

// centroid: with MSAA an edge pixel is shaded when any sample is covered, and at its centre (possibly outside
// the polygon) the UV would run into the neighbouring atlas tile or sprite frame
layout(location = 0) centroid out vec2  fragUV;
layout(location = 1) centroid out float fragShade;

void main()
{
    gl_Position = vec4(inPosW.xy * inPosW.z, 0.0, inPosW.z);
    fragUV    = inUV;
    fragShade = inShade;
}