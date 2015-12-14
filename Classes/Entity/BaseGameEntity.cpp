#include "BaseGameEntity.h"

#include <limits>
#include <Box2D/Box2D.h>
#include "Helper.h"
#include "GB2ShapeCache.h"
#include "ShapeCategory.h"
#include "MessageDispatcher.h"
using namespace cocos2d;


int BaseGameEntity::s_next_entity_id_ = 0;

BaseGameEntity::BaseGameEntity(std::shared_ptr<b2World> world)
	: world_(world)
	, hit_point_(0)
	, run_speed_(0.0f)
	, walk_speed_(0.0f)
	, jump_force_(0.0f)
	, jump_height_(0.0f)
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

	b2BodyDef def;
	def.userData = this;
	def.allowSleep = false;
	def.type = b2_staticBody;
	collision_body_ = world_->CreateBody(&def);

	return true;
}

// 处理消息
void BaseGameEntity::handleMenssage(const Telegram &msg)
{
}

// 根据当前显示帧更新碰撞体
void BaseGameEntity::update_collision_body_by_spriteframe()
{
	if (collision_body_ != nullptr)
	{
		// 销毁所有形状
		b2Fixture *fixture = collision_body_->GetFixtureList();
		while (fixture != nullptr)
		{
			b2Fixture *next_fixture = fixture->GetNext();
			collision_body_->DestroyFixture(fixture);
			fixture = next_fixture;
		}

		// 创建新的形状
		std::string shape = Splitext(getSpriteFrame()->getPolygonInfo().filename)[0];
		if (!shape.empty())
		{
			// 添加身体形状
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape);
			// 添加武器形状
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape + "_weapon");
		}

		// 是否翻转刚体
		if (isFlippedX())
		{
			for (b2Fixture *fixture = collision_body_->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
			{
				if (b2Shape::e_polygon == fixture->GetShape()->m_type)
				{
					b2PolygonShape *shape = dynamic_cast<b2PolygonShape *>(fixture->GetShape());
					for (int i = 0; i < shape->GetVertexCount(); ++i)
					{
						shape->m_vertices[i].x = -shape->m_vertices[i].x;
					}
				}
			}
		}
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

// 获取刚体指针
b2Body* BaseGameEntity::get_body()
{
	return collision_body_;
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

// 更新刚体位置
void BaseGameEntity::updateBodyPosition()
{
	if (collision_body_ != nullptr)
	{
		const float PTMRatio = GB2ShapeCache::instance()->getPTMRatio();
		collision_body_->SetTransform(b2Vec2(getPositionX() / PTMRatio, getPositionY() / PTMRatio), collision_body_->GetAngle());
	}
}

// 获取命中的目标
std::vector<BaseGameEntity*> BaseGameEntity::getHitTargets() const
{
	std::vector<BaseGameEntity*> targets;
	if (collision_body_ != nullptr)
	{
		b2ContactEdge *contactEdge = collision_body_->GetContactList();
		while (contactEdge != nullptr)
		{
			b2Fixture *fixtureA = contactEdge->contact->GetFixtureA();
			b2Fixture *fixtureB = contactEdge->contact->GetFixtureB();
			if (fixtureA->GetBody() == collision_body_ &&
				fixtureA->GetFilterData().categoryBits == ShapeCategory::shape_hero_weapon)
			{
				targets.push_back(reinterpret_cast<BaseGameEntity*>(fixtureB->GetBody()->GetUserData()));
			}
			else if (fixtureB->GetBody() == collision_body_ &&
					 fixtureB->GetFilterData().categoryBits == ShapeCategory::shape_hero_weapon)
			{
				targets.push_back(reinterpret_cast<BaseGameEntity*>(fixtureA->GetBody()->GetUserData()));
			}
			contactEdge = contactEdge->next;
		}
	}
	return targets;
}

void BaseGameEntity::setPositionX(float x)
{
	Sprite::setPositionX(x);
	updateBodyPosition();
}

void BaseGameEntity::setPositionY(float y)
{
	Sprite::setPositionY(y);
	updateBodyPosition();
}

void BaseGameEntity::setPosition(const Vec2 &pos)
{
	Sprite::setPosition(pos);
	updateBodyPosition();
}

void BaseGameEntity::setPosition(float x, float y)
{
	Sprite::setPosition(x, y);;
	updateBodyPosition();
}