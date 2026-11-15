// anaglyph.frag
// The two eyes' images combined for coloured glasses (red / cyan by default): each eye is seen through its filter, so
// each is drawn through that filter's colour, and the two are added.
//   Day levels: a "colour" anaglyph, both eyes in their own colours, as the original game's stereo mode.
//   Night and cave levels (filters.ghost.y = 1): a "half-colour" anaglyph, the left eye as its brightness. The left
//   (red) lens only passes red, so in its own colours the left eye would see little of a dark, blue scene and the depth
//   would be lost; as brightness it sees the whole scene, and the right eye still keeps the scene's colours.
#version 450

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 0) uniform sampler2D leftEye;
layout(set = 1, binding = 0) uniform sampler2D rightEye;

layout(push_constant) uniform Filters
{
    vec4 leftColour;    // rgb: the left lens' colour
    vec4 rightColour;   // rgb: the right lens' colour
    vec4 ghost;         // x: how much of the other eye's image each lens lets through (0 = no ghost cancellation)
                        // y: 1 = half-colour (a dark level), 0 = full colour
} filters;

const vec3 kLuma = vec3(0.299, 0.587, 0.114);

void main()
{
    vec3 leftRgb = texture(leftEye, inUV).rgb;
    vec3 right = texture(rightEye, inUV).rgb;
    float halfColour = step(0.5, filters.ghost.y);
    vec3 left = mix(leftRgb, vec3(dot(leftRgb, kLuma)), halfColour);
    // What of the right eye's image leaks to the left eye: as seen the same way as the left eye's own
    vec3 rightLeak = mix(right, vec3(dot(right, kLuma)), halfColour);

    // Ghost cancellation: each lens lets through k of the other eye's image, so k of the other eye's image is taken
    // off each eye's beforehand and the leak fills it back in. Light can't go below black, so black is raised to k
    // (a little less contrast) to leave room for the subtraction.
    float k = clamp(filters.ghost.x, 0.0, 0.5);
    vec3 leftShown = clamp(k + (1.0 - k) * left - k * rightLeak, 0.0, 1.0);
    vec3 rightShown = clamp(k + (1.0 - k) * right - k * left, 0.0, 1.0);

    outColor = vec4(clamp(leftShown * filters.leftColour.rgb + rightShown * filters.rightColour.rgb, 0.0, 1.0), 1.0);
}
