// lenticular.frag
// Interleaves the quilt for a Looking Glass display: each subpixel of the screen shows the view its lenticule sends
// it towards (the HoloPlay lightfield mapping). Drawn over the whole window, which is the display.
#version 450

layout(location = 0) in vec2 inUV;            // 0..1 over the window, from the top left
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D uQuilt;   // columns x rows views, view 0 the bottom left cell

layout(push_constant) uniform PushConstants
{
	float pitch;     // lenticules across the screen
	float tilt;      // phase change over the screen's height (per unit of pitch)
	float center;    // phase offset
	float subp;      // one subpixel, in screen widths
	int invView;     // 1 = the views run the other way
	int ri;          // which of the three samples feeds red, and blue
	int bi;
	int columns;
	int rows;
	int flipX;
} pc;

// The point uv (0..1, from the bottom left) of the view at phase 0..1
vec2 QuiltUV(vec2 uv, float phase)
{
	const float viewCount = float(pc.columns * pc.rows);
	const float view = min(floor(phase * viewCount), viewCount - 1.0);
	const float x = (mod(view, float(pc.columns)) + uv.x) / float(pc.columns);
	const float y = (floor(view / float(pc.columns)) + uv.y) / float(pc.rows);
	return vec2(x, 1.0 - y);   // the quilt's rows count from its bottom, the image's v from its top
}

void main()
{
	// The lightfield maths is from the bottom left of the screen
	const vec2 screen = vec2(inUV.x, 1.0 - inUV.y);
	const vec2 viewUV = vec2(pc.flipX != 0 ? 1.0 - screen.x : screen.x, screen.y);

	vec3 samples[3];
	for (int i = 0; i < 3; i++)
	{
		float phase = (screen.x + float(i) * pc.subp + screen.y * pc.tilt) * pc.pitch - pc.center;
		phase = fract(phase);
		if (pc.invView != 0)
			phase = 1.0 - phase;
		samples[i] = texture(uQuilt, QuiltUV(viewUV, phase)).rgb;
	}
	outColor = vec4(samples[pc.ri].r, samples[1].g, samples[pc.bi].b, 1.0);
}
