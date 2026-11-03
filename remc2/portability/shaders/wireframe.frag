#version 450

layout(location = 0) in float inBrightness;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(vec3(inBrightness), 1.0);
}