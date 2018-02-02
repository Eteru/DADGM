
attribute vec2 a_posL;
attribute vec2 a_uv;

uniform vec2 u_tex_offset;
uniform mat4 u_matrix;

varying vec2 v_uv;

void main()
{
	v_uv = a_uv + u_tex_offset;
	gl_Position = u_matrix * vec4(a_posL, 0.0, 1.0);
}
   