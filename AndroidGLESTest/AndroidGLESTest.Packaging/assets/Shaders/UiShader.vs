
attribute vec2 a_posL;

//varying v_text_coords;

//uniform u_matrix;

void main()
{
	//v_tex_coords = vec2((a_posL.x + 1.0) / 2.0, 1 - (a_posL.y + 1.0) / 2.0);

	gl_Position = vec4(a_posL, 0.0, 1.0);
}
   