
attribute vec3 a_posL;

uniform mat4 u_matrix;

void main()
{
	vec4 posL = u_matrix * vec4(a_posL, 1.0);

	gl_Position = posL;
}
   