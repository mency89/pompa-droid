#include "EntityManger.h"

#include <Box2D/Box2D.h>
#include "Hero.h"
#include "Boss.h"


EntityManger::EntityManger(std::shared_ptr<b2World> world)
	: world_(world)
{
	CCAssert(world_ != nullptr, "");
}

EntityManger::~EntityManger()
{
	destroyAllEntity();
}

// 创建实例
BaseGameEntity* EntityManger::create(EntityType type)
{
	BaseGameEntity *entity = nullptr;
	switch (type)
	{
		case entity_hero:
			entity = GameEntityCreator<Hero>().create(world_);
			break;
		case entity_boss:
			entity = GameEntityCreator<Boss>().create(world_);
			break;
		default:
			return nullptr;
	}

	if (entity != nullptr)
	{
		entitys_.insert(std::make_pair(entity->getID(), entity));
		entity->retain();
		entity->setEntityManger(this);
	}

	return entity;
}

// 通过id获取实例
BaseGameEntity* EntityManger::getEntityByID(int id)
{
	auto result = entitys_.find(id);
	return result != entitys_.end() ? result->second : nullptr;
}

// 销毁实例
void EntityManger::destroyEntity(int id)
{
	destroy_set_.insert(id);
}

// 销毁实例
void EntityManger::destroyEntity(BaseGameEntity *entity)
{
	destroyEntity(entity->getID());
}

// 销毁所有实例
void EntityManger::destroyAllEntity()
{
	for (auto itr = entitys_.begin(); itr != entitys_.end();)
	{
		itr->second->destroyBody();
		itr->second->removeFromParentAndCleanup(true);
		itr->second->release();
		itr = entitys_.erase(itr);
	}
	destroy_set_.clear();
}

// 用于更新实例状态
void EntityManger::update()
{
	// 更新状态
	for (auto itr = entitys_.begin(); itr != entitys_.end(); ++itr)
	{
		itr->second->update();
	}

	// 删除实例
	for (int id : destroy_set_)
	{
		auto result = entitys_.find(id);
		if (result != entitys_.end())
		{
			result->second->destroyBody();
			result->second->removeFromParentAndCleanup(true);
			result->second->release();
			entitys_.erase(result);
		}
	}
	destroy_set_.clear();

	// 更新形状和位置
	for (auto itr = entitys_.begin(); itr != entitys_.end(); ++itr)
	{
		itr->second->updateCollisionBodyBySpriteframe();
	}
}