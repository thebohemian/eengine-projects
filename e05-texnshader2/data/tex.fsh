#version 120

// from host program
uniform float interpol;
uniform float time;

// this as well
uniform sampler2D tex1;
uniform sampler2D tex2;

const float PI = 3.14159265358979323846264;

void main()
{
    // built-in texture coordinate gl_TexCoord[0] is set in the
    // vertex shader

    // retrieve color value from textures
    vec3 color1 = texture2D(tex1, gl_TexCoord[0].st).rgb;
    vec3 color2 = texture2D(tex2, gl_TexCoord[0].st).rgb;

    float i = 0.5 + cos(gl_TexCoord[0].s*2*PI + time)/2;

    // linearly interpolate between the two rgb-tupels
//	gl_FragColor = vec4(color1 + (color2 - color1) * interpol, 1.0);
	gl_FragColor = vec4(color1 + (color2 - color1) * i, 1.0);
}
