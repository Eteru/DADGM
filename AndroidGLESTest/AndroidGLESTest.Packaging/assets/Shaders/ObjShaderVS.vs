precision mediump float;
attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_eye;
varying vec3 v_pos;
varying vec4 v_color;

uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_matrix;

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

void main()
{
	vec4 posL = u_matrix * vec4(a_posL, 1.0);
	
	v_normal = normalize(u_nm * vec4(a_normal, 1.0)).rgb;

	vec4 pos = u_vm * vec4(a_posL, 1.0);

	v_eye = -pos.rgb;

	v_color = vec4(a_color.rgb, 1.0);
	v_uv = a_uv;

	v_pos = pos.xyz;
	gl_Position = posL;
}
   