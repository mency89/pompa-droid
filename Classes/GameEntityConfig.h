#ifndef __GAMEENTITYCONFIG_H__
#define __GAMEENTITYCONFIG_H__

#include "Singleton.h"

struct EntityAttribute
{
	float width;
	float height;
	float walk_speed;
	float run_speed;
	float jump_force;
	float max_jump_height;

	EntityAttribute()
		: width(0)
		, height(0)
		, walk_speed(0)
		, run_speed(0)
		, jump_force(0)
		, max_jump_height(0)
	{
	}
};

class GameEntityConfig : Singleton < GameEntityConfig >
{
	SINGLETON(GameEntityConfig);

private:
	void loadConfigFile(const std::string &filename);
};

#endif