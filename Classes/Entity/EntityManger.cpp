#include "EntityManger.h"

#include <Box2D/Box2D.h>
#include "Hero.h"
#include "Boss.h"
#include "Robot.h"
#include "Trashcan.h"
#include "Scene/LevelLayer.h"


EntityManger::EntityManger(std::shared_ptr<b2World> world, LevelLayer *level)
	: world_(world)
	, current_level_(level)
{
	CCAssert(world_ != nullptr, "");
	CCAssert(current_level_ != nullptr, "");
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
		case kEntityHero:
			entity = GameEntityCreator<Hero>().create(type, world_);
			break;
		case kEntityRobot:
			entity = GameEntityCreator<Robot>().create(type, world_);
			break;
		case kEntityBoss:
			entity = GameEntityCreator<Boss>().create(type, world_);
			break;
		case kEntityTrashcan:
			entity = GameEntityCreator<Trashcan>().create(type, world_);
			break;
		default:
			return nullptr;
	}

	if (entity != nullptr)
	{
		entitys_.insert(std::make_pair(entity->getID(), entity));
		entity->setEntityManger(this);
		current_level_->addChild(entity, current_level_->layerCount());
		entity->retain();
	}

	return entity;
}

// 通过id获取实例
BaseGameEntity* EntityManger::getEntityByID(int id)
{
	auto result = entitys_.find(id);
	return result != entitys_.end() ? result->second : nullptr;
}

// 获取所有实例
std::vector<BaseGameEntity*> EntityManger::getAllEntitys()
{
	std::vector<BaseGameEntity*> ret;
	for (auto itr = entitys_.begin(); itr != entitys_.end(); ++itr)
	{
		ret.push_back(itr->second);
	}
	return ret;
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

// 获取当前关卡
LevelLayer* EntityManger::getCurrentLevel()
{
	return current_level_;
}

// 销毁所有实例
void EntityManger::destroyAllEntity()
{
	for (auto itr = entitys_.begin(); itr != entitys_.end();)
	{
		itr->second->destroyBody();
		current_level_->removeChild(itr->second, true);
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
		if (!itr->second->isDeath())
		{
			itr->second->update();
		}
	}

	// 删除实例
	for (int id : destroy_set_)
	{
		auto result = entitys_.find(id);
		if (result != entitys_.end())
		{
			result->second->destroyBody();
			current_level_->removeChild(result->second, true);
			result->second->release();
			entitys_.erase(result);
		}
	}
	destroy_set_.clear();

	// 更新形状和位置
	for (auto itr = entitys_.begin(); itr != entitys_.end(); ++itr)
	{
		if (!itr->second->isDeath())
		{
			itr->second->updateCollisionBodyBySpriteframe();
		}
	}
}