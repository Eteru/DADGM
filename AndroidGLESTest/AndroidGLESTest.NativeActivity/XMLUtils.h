#pragma once
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "Math.h"

namespace XMLUtils
{
	bool GetBoolValueSafe(const rapidxml::xml_node<> *pNode, const bool defaultValue);
	bool GetBoolValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const bool defaultValue);

	std::string GetStringValue(const rapidxml::xml_node<> *pNode);
	std::string GetStringValue(const rapidxml::xml_node<> *pNode, const std::string childName);
	std::string GetStringValueSafe(const rapidxml::xml_node<> *pNode, const std::string defaultValue);
	std::string GetStringValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const std::string defaultValue);

	GLfloat GetFloatValue(const rapidxml::xml_node<> *pNode);
	GLfloat GetFloatValue(const rapidxml::xml_node<> *pNode, const std::string childName);
	GLfloat GetFloatValueSafe(const rapidxml::xml_node<> *pNode, const GLfloat defaultValue);
	GLfloat GetFloatValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const GLfloat defaultValue);

	int GetIntValue(const rapidxml::xml_node<> *pNode);
	int GetIntValue(const rapidxml::xml_node<> *pNode, const std::string childName);
	int GetIntValueSafe(const rapidxml::xml_node<> *pNode, const int defaultValue);
	int GetIntValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const int defaultValue);

	Vector3 GetVectorValueXYZ(const rapidxml::xml_node<> *pNode);
	Vector3 GetVectorValueXYZ(const rapidxml::xml_node<> *pNode, const std::string childName);
	Vector3 GetVectorValueXYZSafe(const rapidxml::xml_node<> *pNode, const Vector3 defaultValue);
	Vector3 GetVectorValueXYZSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const Vector3 defaultValue);

	Vector3 GetVectorValueRGB(const rapidxml::xml_node<> *pNode);
	Vector3 GetVectorValueRGB(const rapidxml::xml_node<> *pNode, const std::string childName);
	Vector3 GetVectorValueRGBSafe(const rapidxml::xml_node<> *pNode, const Vector3 defaultValue);
	Vector3 GetVectorValueRGBSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const Vector3 defaultValue);

	Vector2 GetVec2ValueXYSafe(const rapidxml::xml_node<> *pNode, const Vector2 defaultValue);
	Vector2 GetVec2ValueXYSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const Vector2 defaultValue);
}