#include "XMLUtils.h"

bool XMLUtils::GetBoolValueSafe(const rapidxml::xml_node<> *pNode, const bool defaultValue)
{
	return nullptr == pNode ? defaultValue : std::string(pNode->value()) == "true";
}

bool XMLUtils::GetBoolValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const bool defaultValue)
{
	return nullptr == pNode ? defaultValue : GetBoolValueSafe(pNode->first_node(childName.c_str()), defaultValue);
}

std::string XMLUtils::GetStringValue(const rapidxml::xml_node<> *pNode)
{
	return std::string(pNode->value());
}

std::string XMLUtils::GetStringValue(const rapidxml::xml_node<> *pNode, const std::string childName)
{
	return pNode->first_node(childName.c_str())->value();
}

std::string XMLUtils::GetStringValueSafe(const rapidxml::xml_node<> *pNode, const std::string defaultValue)
{
	return nullptr == pNode ? defaultValue : GetStringValue(pNode);
}

std::string XMLUtils::GetStringValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const std::string defaultValue)
{
	return nullptr == pNode ? defaultValue : GetStringValueSafe(pNode->first_node(childName.c_str()), defaultValue);
}

GLfloat XMLUtils::GetFloatValue(const rapidxml::xml_node<> *pNode)
{
	return strtof(pNode->value(), 0);
}

GLfloat XMLUtils::GetFloatValue(const rapidxml::xml_node<> *pNode, const std::string childName)
{
	return strtof(pNode->first_node(childName.c_str())->value(), 0);
}

GLfloat XMLUtils::GetFloatValueSafe(const rapidxml::xml_node<> *pNode, const GLfloat defaultValue)
{
	return nullptr == pNode ? defaultValue : GetFloatValue(pNode);
}

GLfloat XMLUtils::GetFloatValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const GLfloat defaultValue)
{
	return nullptr == pNode ? defaultValue : GetFloatValueSafe(pNode->first_node(childName.c_str()), defaultValue);
}

int XMLUtils::GetIntValue(const rapidxml::xml_node<> *pNode)
{
	return atoi(pNode->value());
}

int XMLUtils::GetIntValue(const rapidxml::xml_node<> *pNode, const std::string childName)
{
	return atoi(pNode->first_node(childName.c_str())->value());
}

int XMLUtils::GetIntValueSafe(const rapidxml::xml_node<> *pNode, const int defaultValue)
{
	return nullptr == pNode ? defaultValue : GetIntValue(pNode);
}

int XMLUtils::GetIntValueSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const int defaultValue)
{
	return nullptr == pNode ? defaultValue : GetIntValueSafe(pNode->first_node(childName.c_str()), defaultValue);
}

Vector3 XMLUtils::GetVectorValueXYZ(const rapidxml::xml_node<> *pNode)
{
	return Vector3(GetFloatValue(pNode, "x"), GetFloatValue(pNode, "y"), GetFloatValue(pNode, "z"));
}

Vector3 XMLUtils::GetVectorValueXYZ(const rapidxml::xml_node<> *pNode, const std::string childName)
{
	return GetVectorValueXYZ(pNode->first_node(childName.c_str()));
}

Vector3 XMLUtils::GetVectorValueXYZSafe(const rapidxml::xml_node<> *pNode, const Vector3 defaultValue)
{
	return nullptr == pNode ? defaultValue : Vector3(GetFloatValueSafe(pNode->first_node("x"), defaultValue.x), GetFloatValueSafe(pNode->first_node("y"), defaultValue.y), GetFloatValueSafe(pNode->first_node("z"), defaultValue.z));
}

Vector3 XMLUtils::GetVectorValueXYZSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const Vector3 defaultValue)
{
	return nullptr == pNode ? defaultValue : GetVectorValueXYZSafe(pNode->first_node(childName.c_str()), defaultValue);
}

Vector3 XMLUtils::GetVectorValueRGB(const rapidxml::xml_node<> *pNode)
{
	return Vector3(GetFloatValue(pNode, "r"), GetFloatValue(pNode, "g"), GetFloatValue(pNode, "b"));
}

Vector3 XMLUtils::GetVectorValueRGB(const rapidxml::xml_node<> *pNode, const std::string childName)
{
	return GetVectorValueRGB(pNode->first_node(childName.c_str()));
}

Vector3 XMLUtils::GetVectorValueRGBSafe(const rapidxml::xml_node<> *pNode, const Vector3 defaultValue)
{
	return nullptr == pNode ? defaultValue : Vector3(GetFloatValueSafe(pNode->first_node("r"), defaultValue.x), GetFloatValueSafe(pNode->first_node("g"), defaultValue.y), GetFloatValueSafe(pNode->first_node("b"), defaultValue.z));
}

Vector3 XMLUtils::GetVectorValueRGBSafe(const rapidxml::xml_node<> *pNode, const std::string childName, const Vector3 defaultValue)
{
	return nullptr == pNode ? defaultValue : GetVectorValueRGBSafe(pNode->first_node(childName.c_str()), defaultValue);
}