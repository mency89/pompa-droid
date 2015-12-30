﻿#ifndef __GAMEENTITYCONFIG_H__
#define __GAMEENTITYCONFIG_H__

#include <map>
#include <string>
#include "cocos2d.h"
#include "Singleton.h"

struct EntityAttribute
{
	float walk_speed;
	float run_speed;
	float jump_force;
	float max_jump_height;
	cocos2d::Size size;
	cocos2d::Size real_size;

	EntityAttribute()
		: walk_speed(0)
		, run_speed(0)
		, jump_force(0)
		, max_jump_height(0)
	{
	}
};

class GameEntityConfig : public Singleton < GameEntityConfig >
{
	SINGLETON(GameEntityConfig);

public:
	bool getEntityAttribute(const std::string &name, const EntityAttribute *&ret);

private:
	void loadConfigFile(const std::string &filename);

private:
	std::map<std::string, EntityAttribute> entitys_;
};

#endif