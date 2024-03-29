﻿#ifndef __GAMEENTITYCREATOR_H__
#define __GAMEENTITYCREATOR_H__

class b2World;

template <typename GameEntity>
struct GameEntityCreator
{
	static GameEntity* create(int type, std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(type, world);
		if (entity && entity->init())
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}
};

#endif