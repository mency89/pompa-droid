#include "GameEntityConfig.h"

#include "cocos2d.h"
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

}