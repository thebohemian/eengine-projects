uniform sampler2D sampler;

void main()
{
	float s = gl_TexCoord[0].s;
	float t = gl_TexCoord[0].t;
	float step = 1.0 / 256.0;
	vec3 lum = vec3(0.30, 0.59, 0.11);
	
	/* Get the first row */
	float a11 = dot(lum, texture2D(sampler, vec2(s - step, t - step)).rgb);
	float a12 = dot(lum, texture2D(sampler, vec2(s,        t - step)).rgb);
	float a13 = dot(lum, texture2D(sampler, vec2(s + step, t - step)).rgb);

	/* Get the second row */
	float a21 = dot(lum, texture2D(sampler, vec2(s - step, t)).rgb);
	float a22 = dot(lum, texture2D(sampler, vec2(s,        t)).rgb);
	float a23 = dot(lum, texture2D(sampler, vec2(s + step, t)).rgb);

	/* Get the third row */
	float a31 = dot(lum, texture2D(sampler, vec2(s - step, t + step)).rgb);
	float a32 = dot(lum, texture2D(sampler, vec2(s,        t + step)).rgb);
	float a33 = dot(lum, texture2D(sampler, vec2(s + step, t + step)).rgb);

	float yComp = a11 + 2.0 * a12 + a13 - a31 - 2.0 * a32 - a33;
	float xComp = a11 + 2.0 * a21 + a31 - a13 - 2.0 * a23 - a33;
	
	float color = sqrt(xComp * xComp + yComp * yComp);

	gl_FragColor = vec4(color, color, color, 1.0);
}
