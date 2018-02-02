#pragma once
#include "XMLUtils.h"
#include <unordered_map>
#include <vector>
#include "Structs.h"
#include "Item.h"

#include "Robot.h"

//#include "ItemDescriptions.h"


static const std::string configsPath = "XMLs/allConfigs.xml";


class MapDescription;

class XMLParser
{
public:



	static MapDescription GetRandomMapDescription();
	static Robot *GetRandomRobot(const Vector2 mapCoords, MapManager *mapManager);




	static void ParseMapLines(rapidxml::xml_node<> *pNode, Vector2 dims, std::vector<std::string> &mapString);
	static void ParseMapLine(rapidxml::xml_node<> *pNode, Vector2 dims, std::vector<std::string> &mapString);
	static std::vector<rapidxml::xml_node<> *> GetAllChildren(rapidxml::xml_node<> *node, const std::string name);
	static rapidxml::xml_node<> *GetRootOfFile(std::string filePath);
private:
	static MapDescription GetMapDescription(rapidxml::xml_node<> *mapNode);

	static Robot *GetRobot(rapidxml::xml_node<> *robotNode, std::vector<rapidxml::xml_node<> *> allArmors, std::vector<rapidxml::xml_node<> *> allWeapons, std::vector<rapidxml::xml_node<> *> allItems);


	static void AddAllStats(StatHolderObject *obj, rapidxml::xml_node<> *statNodeRoot);
	static void AddStat(StatHolderObject *obj, rapidxml::xml_node<> *statNode);

	static Armor *ParseArmor(rapidxml::xml_node<> *armorNode, std::vector<rapidxml::xml_node<> *> allArmors);
	static Weapon *ParseWeapon(rapidxml::xml_node<> *weaponNode, std::vector<rapidxml::xml_node<> *> allWeapons);



	template<typename T> static void SafeDeleteVec(std::vector<T*> vec);


	static std::pair<rapidxml::xml_node<> *, char *> m_crtRoot;
};


