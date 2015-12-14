#include "BaseGameEntity.h"

#include <limits>
#include <Box2D/Box2D.h>
#include "MessageDispatcher.h"
using namespace cocos2d;


int BaseGameEntity::s_next_entity_id_ = 0;

BaseGameEntity::BaseGameEntity(std::shared_ptr<b2World> world)
	: world_(world)
	, run_speed_(0.0f)
	, walk_speed_(0.0f)
	, jump_force_(0.0f)
	, jump_height_(0.0f)
	, collision_body_(nullptr)
	, collision_weapon_(nullptr)
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

// 获取唯一id
int BaseGameEntity::get_id() const
{
	return entity_id_;
}

// 移动
void BaseGameEntity::move(float speed)
{
	if (direction_ == Direction::Up)
	{
		setPosition(getPosition() + Vec2(0, speed));
	}
	else if (direction_ == Direction::Down)
	{
		setPosition(getPosition() - Vec2(0, speed));
	}
	else if (direction_ == Direction::Left)
	{
		setPosition(getPosition() - Vec2(speed, 0));
	}
	else
	{
		setPosition(getPosition() + Vec2(speed, 0));
	}
}


// 获取跑动速度
float BaseGameEntity::getRunSpeed() const
{
	return run_speed_;
}

// 设置跑动速度
void BaseGameEntity::setRunSpeed(float speed)
{
	run_speed_ = speed;
}

// 获取行走速度
float BaseGameEntity::getWalkSpeed() const
{
	return walk_speed_;
}

// 设置行走速度
void BaseGameEntity::setWalkSpeed(float speed)
{
	walk_speed_ = speed;
}

// 获取跳跃力
float BaseGameEntity::getJumpForce() const
{
	return jump_force_;
}

// 设置跳跃力
void BaseGameEntity::setJumpForce(float force)
{
	jump_force_ = force;
}

// 获取最大跳跃高度
float BaseGameEntity::getMaxJumpHeight() const
{
	return jump_height_;
}

// 设置最大跳跃高度
void BaseGameEntity::setMaxJumpHeight(float height)
{
	jump_height_ = height;
}

// 获取方向
BaseGameEntity::Direction BaseGameEntity::getDirection() const
{
	return direction_;
}
// 设置方向
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