#include "BaseGameEntity.h"

#include <limits>
#include <Box2D/Box2D.h>
#include "MessageDispatcher.h"
using namespace cocos2d;


int BaseGameEntity::s_next_entity_id_ = 0;

BaseGameEntity::BaseGameEntity(std::shared_ptr<b2World> world)
	: world_(world)
	, collision_body_(nullptr)
	, entity_id_(++s_next_entity_id_)
{
	CCAssert(world_ != nullptr, "");
	if (s_next_entity_id_ == std::numeric_limits<int>::max())
	{
		s_next_entity_id_ = 0;
	}
}

BaseGameEntity::~BaseGameEntity()
{
	MessageDispatcher::instance()->unregisterEntity(this);
}

bool BaseGameEntity::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	setDirection(Direction::Right);
	MessageDispatcher::instance()->registerEntity(this);

	return true;
}

void BaseGameEntity::handleMenssage(const Telegram &msg)
{

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

int BaseGameEntity::get_id() const
{
	return entity_id_;
}

BaseGameEntity::Direction BaseGameEntity::getDirection() const
{
	return direction_;
}

void BaseGameEntity::setDirection(Direction direction)
{
	direction_ = direction;
	if (direction_ == Direction::Right)
	{
		setFlippedX(false);
	}
	else if (direction_ == Direction::Left)
	{
		setFlippedX(true);
	}
}