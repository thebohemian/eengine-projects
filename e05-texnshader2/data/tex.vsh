varying vec2 texture_coordinate;

uniform float time;

void main()
{
    vec4 v = vec4(gl_Vertex);
    v.y = v.y + sin(time)*50;

	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * v;

	// passing texture coordinate of texture unit 0
	// to fragment shader
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

