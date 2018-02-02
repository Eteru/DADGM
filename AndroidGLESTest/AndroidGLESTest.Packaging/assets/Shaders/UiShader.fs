precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_tex;
uniform vec4 u_color;

void main()
{
	vec4 col = texture2D(u_tex, v_uv);
	if (col.a < 0.1)
	{
		discard;
	}

	gl_FragColor = col;
}
