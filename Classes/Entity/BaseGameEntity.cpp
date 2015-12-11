#include "BaseGameEntity.h"

#include <Box2D/Box2D.h>
using namespace cocos2d;


BaseGameEntity::BaseGameEntity(std::shared_ptr<b2World> world)
	: world_(world)
	, collision_body_(nullptr)
{
	CCAssert(world_ != nullptr, "");
}

BaseGameEntity::~BaseGameEntity()
{

}

bool BaseGameEntity::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	return true;
}

void BaseGameEntity::update_collision_body_by_spriteframe()
{
	if (collision_body_ != nullptr)
	{
		world_->DestroyBody(collision_body_);
		collision_body_ = nullptr;
	}
}

void BaseGameEntity::update()
{
	update_collision_body_by_spriteframe();
}