#include "GameEntityConfig.h"

#include "Helper.h"
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
				attrubute.walk_speed = object["walk_speed"].GetDouble();
				attrubute.run_speed = object["run_speed"].GetDouble();
				attrubute.jump_force = object["jump_force"].GetDouble();
				attrubute.max_jump_height = object["max_jump_height"].GetDouble();
				attrubute.attack = object["attack"].GetUint();
				attrubute.run_attack = object["run_attack"].GetUint();
				attrubute.jump_attack = object["jump_attack"].GetUint();
				attrubute.max_hit_point = object["max_hit_point"].GetUint();
				
				auto size = Split(object["size"].GetString(), ",");
				auto real_size = Split(object["real_size"].GetString(), ",");
				CCAssert(size.size() == 2 && real_size.size() == 2, "");
				attrubute.size = Size(atoi(size[0].c_str()), atoi(size[1].c_str()));
				attrubute.real_size = Size(atoi(real_size[0].c_str()), atoi(real_size[1].c_str()));

				entitys_.insert(std::make_pair(name, attrubute));
			}
		}
	}
}

bool GameEntityConfig::getEntityAttribute(const std::string &name, const EntityAttribute *&ret)
{
	auto itr = entitys_.find(name);
	if (itr != entitys_.end())
	{
		ret = &itr->second;
		return true;
	}
	return false;
}