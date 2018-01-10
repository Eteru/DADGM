#pragma once

#include <string>
#include <android/log.h>
#include "Math.h"

#define  LOG_TAG    "LOG_DEBUG"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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

struct BoundingBox
{
	Vector3 bb_min;
	Vector3 bb_max;

	BoundingBox()
		: bb_min(Vector3(0, 0, 0)), bb_max(Vector3(0, 0, 0))
	{}

	BoundingBox(Vector3 min_p, Vector3 max_p)
		: bb_min(min_p), bb_max(max_p)
	{}

	inline bool Contains(const Vector3 & point) const
	{
		if (bb_min.x > point.x) return false;
		if (bb_min.y > point.y) return false;
		if (bb_min.z > point.z) return false;
		if (bb_max.x < point.x) return false;
		if (bb_max.y < point.y) return false;
		if (bb_max.z < point.z) return false;

		return true;
	}

	inline bool Collides(const BoundingBox & rhs_bb) const
	{
		if (bb_min.x > rhs_bb.bb_max.x) return false;
		if (bb_min.y > rhs_bb.bb_max.y) return false;
		if (bb_min.z > rhs_bb.bb_max.z) return false;
		if (bb_max.x < rhs_bb.bb_min.x) return false;
		if (bb_max.y < rhs_bb.bb_min.y) return false;
		if (bb_max.z < rhs_bb.bb_min.z) return false;

		return true;
	}
};