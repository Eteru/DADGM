#pragma once
#include <string>



#define LOG_TAG "TAGUL_NOSTRU"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace PrintUtils
{
	std::string ToString(const int value);
	std::string ToString(const size_t value);
	std::string ToString(const GLfloat value);

	void PrintD(const std::string &string);
	void PrintI(const std::string &string);
}
