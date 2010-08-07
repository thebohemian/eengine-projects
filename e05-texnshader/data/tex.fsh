#version 120

uniform float r;
uniform float g;
uniform float b;

void main(void)
{
	gl_FragColor = vec4(r, g, b, 1.0);
}

