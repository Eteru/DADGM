#pragma once
#include <unordered_map>
#include <unordered_set>

#include "PrintUtils.h"

static std::unordered_map<std::string, std::pair<size_t, std::unordered_set<size_t>>> classIDsMap;


class UniqueID
{
public:


	// Daca incarcam obiecte din XML, folosim isBlacklisted si apoi blackList, ca ID-urile se dau de acolo
	static bool IsBlacklisted(const std::string className, const size_t ID)
	{
		if (0 == classIDsMap.count(className))
			return false;

		return classIDsMap.at(className).second.count(ID) >= 1;
	}

	static void BlacklistID(std::string className, size_t ID)
	{
		PrintUtils::PrintI("Blacklisting (" + className + ", " + PrintUtils::ToString(ID) + ")");

		if (0 == classIDsMap.count(className))
		{
			std::pair<size_t, std::unordered_set<size_t>> setStruct = std::make_pair(ID, std::unordered_set<size_t>(ID));
			classIDsMap[className] = setStruct;
		}
		else
		{
			classIDsMap.at(className).second.insert(ID);
			classIDsMap.at(className).first = std::max(ID, classIDsMap.at(className).first);
		}		
	}


	// Daca cream obiecte din cod, folosim GetID
	static size_t GetID(const std::string &className)
	{
		if (0 == classIDsMap.count(className))
		{
			classIDsMap[className].first = 0;
			classIDsMap[className].second.insert(0);

			return 0;
		}


		std::pair<size_t, std::unordered_set<size_t>> *setStruct = &classIDsMap[className];
		setStruct->first++;
		setStruct->second.insert(setStruct->first);

		return setStruct->first;
	}

//private:


};