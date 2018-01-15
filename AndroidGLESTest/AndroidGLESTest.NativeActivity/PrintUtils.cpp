#include <sstream>

#include "PrintUtils.h"
#include "Structs.h"

std::string PrintUtils::ToString(const size_t value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string PrintUtils::ToString(const int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string PrintUtils::ToString(const GLfloat value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

void PrintUtils::PrintD(const std::string &string)
{
	LOGD(string.c_str());
}

void PrintUtils::PrintI(const std::string &string)
{
	LOGI(string.c_str());
}
