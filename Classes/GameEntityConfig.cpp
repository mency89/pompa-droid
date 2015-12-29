#include "GameEntityConfig.h"

#include "cocos2d.h"
#include "json/document.h"
using namespace cocos2d;


GameEntityConfig::GameEntityConfig()
{
	loadConfigFile("config/GameEntity.json");
}

GameEntityConfig::~GameEntityConfig()
{

}

void GameEntityConfig::loadConfigFile(const std::string &filename)
{
	Data data = FileUtils::getInstance()->getDataFromFile(filename);
	if (!data.isNull())
	{
		rapidjson::Document doc;
		std::string buf(reinterpret_cast<char *>(data.getBytes()), data.getSize());
		doc.Parse<0>(buf.c_str());
		if (!doc.HasParseError())
		{
			for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
			{
				EntityAttribute attrubute;
				const rapidjson::Value &object = itr->value;
				const std::string name = itr->name.GetString();
				attrubute.width = object["width"].GetDouble();
				attrubute.height = object["height"].GetDouble();
				attrubute.walk_speed = object["walk_speed"].GetDouble();
				attrubute.run_speed = object["run_speed"].GetDouble();
				attrubute.jump_force = object["jump_force"].GetDouble();
				attrubute.max_jump_height = object["max_jump_height"].GetDouble();
				entitys_.insert(std::make_pair(name, attrubute));
			}
		}
	}
}

bool GameEntityConfig::getEntityAttribute(const std::string &name, EntityAttribute *ret)
{
	auto itr = entitys_.find(name);
	if (itr != entitys_.end())
	{
		*ret = itr->second;
		return true;
	}
	return false;
}