precision mediump float;

varying vec3 v_normal;
varying vec3 v_eye;
varying vec3 v_pos;
varying vec4 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;


// fog
uniform float u_fog_alpha;
uniform vec3 u_fog_color;

// lights count
#define MAX_LIGHTS 20
uniform highp int u_lights_count;

// light type
uniform highp int u_light_type[MAX_LIGHTS];
uniform vec3 u_light_pos[MAX_LIGHTS];
uniform vec3 u_light_dir[MAX_LIGHTS];

// ambiental light
uniform vec3 u_lightA;
uniform float u_lightA_ratio;

// diffuse light
uniform vec3 u_lightD[MAX_LIGHTS];
uniform float u_lightD_ratio[MAX_LIGHTS];

// specular light
uniform vec3 u_lightS[MAX_LIGHTS];
uniform float u_lightS_ratio[MAX_LIGHTS];
uniform float u_shininess[MAX_LIGHTS];

// spot_light
uniform float u_cone_angle[MAX_LIGHTS];

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

vec3 SpotLight(vec3 l, vec3 color, vec3 position, float ratio, float angle)
{
	float dist = length(v_pos - position);
	vec3 vDir = normalize(v_pos - position);
	float cosine = dot(l, vDir);
	float cone_cosine = cos(angle);
	float dif = 1.0 - cone_cosine;
	float factor = clamp((cosine - cone_cosine), 0.0, 1.0);

	if (cosine > cone_cosine) {
		return color * ratio * factor / dist;
	}

	return vec3(0.0,0.0,0.0);
}

vec3 AmbientLight()
{
	return u_lightA * clamp(u_lightA_ratio, 0.0, 1.0);
}

vec3 DiffuseLight(vec3 n, vec3 l, vec3 color, float ratio)
{
	return color * ratio * max(dot(n, -l), 0.0);
}

vec3 SpecularLight(vec3 n, vec3 l, vec3 e, vec3 color, float ratio, float shininess)
{
	vec3 spec = vec3(0.0);

	float intensity = max(dot(n, l), 0.0);
	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h, n), 0.0);
		spec = color * pow(intSpec, shininess) * ratio;
	}

	return spec;
}

void main()
{
	vec4 c_1 = texture2D(u_texture_0, v_uv);
	vec4 c_2 = texture2D(u_texture_1, v_uv);
	vec4 c_3 = texture2D(u_texture_2, v_uv);
	vec4 c_A = texture2D(u_texture_3, v_uvBlend);

	vec4 c_final = c_A.r * c_1 + c_A.g * c_2 + c_A.b * c_3;
	c_final.a = 1.0;

	/*vec3 l;
	vec3 e = normalize(v_eye);
	vec3 n = normalize(v_normal);

	vec3 IA = AmbientLight();
	vec3 ID = vec3(0.0), IS = vec3(0.0);

	if (0 < u_lights_count) {
		if (POINT_LIGHT == u_light_type[0]) {
			l = normalize(u_light_pos[0] - v_pos);
		}
		else {
			l = normalize(u_light_dir[0]);
		}

		ID += DiffuseLight(n, l, u_lightD[0], u_lightD_ratio[0]);
		IS += SPOT_LIGHT == u_light_type[0] ? SpotLight(l, u_lightS[0], u_light_pos[0], u_lightS_ratio[0], u_cone_angle[0])
				: SpecularLight(n, l, e, u_lightS[0], u_lightS_ratio[0], u_shininess[0]);	
	}
	
	if (1 < u_lights_count) {
		if (POINT_LIGHT == u_light_type[1]) {
			l = normalize(u_light_pos[1] - v_pos);
		}
		else {
			l = normalize(u_light_dir[1]);
		}

		ID += DiffuseLight(n, l, u_lightD[1], u_lightD_ratio[1]);
		IS += SPOT_LIGHT == u_light_type[1] ? SpotLight(l, u_lightS[1], u_light_pos[1], u_lightS_ratio[1], u_cone_angle[1])
				: SpecularLight(n, l, e, u_lightS[1], u_lightS_ratio[1], u_shininess[1]);	
	}

	if (2 < u_lights_count) {
		if (POINT_LIGHT == u_light_type[2]) {
			l = normalize(u_light_pos[2] - v_pos);
		}
		else {
			l = normalize(u_light_dir[2]);
		}

		ID += DiffuseLight(n, l, u_lightD[2], u_lightD_ratio[2]);
		IS += SPOT_LIGHT == u_light_type[2] ? SpotLight(l, u_lightS[2], u_light_pos[2], u_lightS_ratio[2], u_cone_angle[2])
				: SpecularLight(n, l, e, u_lightS[2], u_lightS_ratio[2], u_shininess[2]);	
	}

	// copy this till you die
	
	ID /= float(u_lights_count);
	IS /= float(u_lights_count);
	
	c_final = c_final * vec4(IA * ID, 1.0) + vec4(IS, 1.0);*/

	float alpha_clamped = clamp(u_fog_alpha, 0.0, 1.0);
	c_final = vec4(alpha_clamped * u_fog_color + (1.0 - alpha_clamped) * c_final.xyz, c_final.a);

	gl_FragColor = c_final;
}
