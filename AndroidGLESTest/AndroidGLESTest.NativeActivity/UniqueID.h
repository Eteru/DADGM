#pragma once
#include <unordered_map>
#include <unordered_set>


static std::unordered_map<std::string, std::pair<size_t, std::unordered_set<size_t>>> classIDsMap;


class UniqueID
{
public:


	// Daca incarcam obiecte din XML, folosim isBlacklisted si apoi blackList, ca ID-urile se dau de acolo
	static bool IsBlacklisted(const std::string className, const size_t ID)
	{
		if (0 == classIDsMap.count(className))
			return false;

		return !classIDsMap[className].second.count(ID);
	}

	static void BlacklistID(std::string className, size_t ID)
	{
		std::pair<size_t, std::unordered_set<size_t>> *setStruct = &classIDsMap[className];

		setStruct->second.insert(ID);
		setStruct->first = std::max(ID, setStruct->first);
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