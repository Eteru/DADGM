
attribute vec2 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;

//uniform u_matrix;

void main()
{
	v_uv = a_uv;

	gl_Position = vec4(a_posL, 0.0, 1.0);
}
   