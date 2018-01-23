#pragma once

#include <string>
#include <android/log.h>
#include "Math.h"



/**
* Our saved state data.
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Shared state for our app.
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
	struct saved_state prev_state;
};

enum ObjectType {
	OT_NORMAL = 0,
	OT_SKYBOX,
	OT_TERRAIN,
	OT_ANIMATED
};

struct ModelResource
{
	std::string id;
	std::string model_path;
};

struct TextureResource
{
	GLenum type;
	GLenum min_filter;
	GLenum mag_filter;
	GLenum wrap_s;
	GLenum wrap_t;
	std::string id;
	std::string texture_path;
};

struct ShaderResource
{
	std::string id;
	std::string vs_path;
	std::string fs_path;
};

struct DebugSettings
{
	std::string cube_id;
	std::string sphere_id;
	std::string shader_id;
};

struct StatIncrease
{
	uint32_t flat;
	float additive_coef;
	float multiplicative_coef;

	StatIncrease()
	{
		flat = 0;
		additive_coef = 0.f;
		multiplicative_coef = 0.f;
	}
};

enum StatType { DEFFENSE = 0, OFFENSE, NUM_STATS_TYPES };