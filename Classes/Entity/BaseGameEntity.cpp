#include "BaseGameEntity.h"

#include <limits>
#include <Box2D/Box2D.h>
#include "Helper.h"
#include "GB2ShapeCache.h"
#include "AnimationManger.h"
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
	, manager_(nullptr)
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
	destroyBody();
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
	def.type = b2_dynamicBody;
	collision_body_ = world_->CreateBody(&def);

	return true;
}

// 受击
void BaseGameEntity::onHurt(const cocos2d::Vec2 &pos)
{
	auto player = Sprite::create();
	player->setPosition(convertToNodeSpace(pos));
	addChild(player);

	Animation *animation = AnimationManger::instance()->getAnimation("hiteffect");
	Animate *animate = Animate::create(animation);
	player->runAction(Sequence::create(animate, CallFunc::create([=]()
	{
		player->removeFromParentAndCleanup(true);
	}), nullptr));
}

// 处理消息
void BaseGameEntity::handleMenssage(const Telegram &msg)
{
}

// 根据当前显示帧更新碰撞体
void BaseGameEntity::updateCollisionBodyBySpriteframe()
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
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape, isFlippedX());
			// 添加武器形状
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape + "_weapon", isFlippedX());
		}
	}
}

// 更新
void BaseGameEntity::update()
{
}

// 获取唯一id
int BaseGameEntity::getID() const
{
	return entity_id_;
}

// 获取刚体
b2Body* BaseGameEntity::getBody()
{
	return collision_body_;
}

// 销毁刚体
void BaseGameEntity::destroyBody()
{
	if (collision_body_ != nullptr)
	{
		collision_body_->GetWorld()->DestroyBody(collision_body_);
		collision_body_ = nullptr;
	}
}

// 获取管理器
EntityManger* BaseGameEntity::getEntityManger()
{
	return manager_;
}

// 设置管理器
void BaseGameEntity::setEntityManger(EntityManger *manager)
{
	manager_ = manager;
}

// 移动实体
void BaseGameEntity::moveEntity(float speed)
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

// 获取面对的方向
BaseGameEntity::Direction BaseGameEntity::getLookDirection() const
{
	return isFlippedX() ? BaseGameEntity::Direction::Left : BaseGameEntity::Direction::Right;
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

// 获取碰撞位置
Vec2 BaseGameEntity::getCollisionPosition(b2CircleShape *source, b2CircleShape *target, b2Body *target_body) const
{
	const float PTMRatio = GB2ShapeCache::instance()->getPTMRatio();

	// 转换世界坐标
	b2Vec2 source_pos = collision_body_->GetPosition() + source->m_p;
	b2Vec2 target_pos = target_body->GetPosition() + target->m_p;

	// 计算碰撞位置
	b2Vec2 collision = target_pos - source_pos;
	collision.Normalize();
	collision.x = collision.x * source->m_radius;
	collision.y = collision.y * source->m_radius;
	collision += source_pos;
	return Vec2(collision.x * PTMRatio, collision.y * PTMRatio);
}

// 获取命中的目标
std::vector<BaseGameEntity::Collision> BaseGameEntity::getHitTargets() const
{
	std::vector<Collision> targets;
	if (collision_body_ != nullptr)
	{
		b2ContactEdge *contactEdge = collision_body_->GetContactList();
		while (contactEdge != nullptr)
		{
			b2Fixture *fixtureA = contactEdge->contact->GetFixtureA();
			b2Fixture *fixtureB = contactEdge->contact->GetFixtureB();
			if (fixtureA->GetShape()->GetType() == b2Shape::e_circle &&
				fixtureB->GetShape()->GetType() == b2Shape::e_circle)
			{
				if (fixtureA->GetBody() == collision_body_ &&
					fixtureA->GetFilterData().categoryBits == weaponCategoryBits())
				{
					b2CircleShape *shapeA = dynamic_cast<b2CircleShape *>(fixtureA->GetShape());
					b2CircleShape *shapeB = dynamic_cast<b2CircleShape *>(fixtureB->GetShape());
					targets.resize(targets.size() + 1);
					targets.back().collision_pos = getCollisionPosition(shapeA, shapeB, fixtureB->GetBody());
					targets.back().entity = reinterpret_cast<BaseGameEntity*>(fixtureB->GetBody()->GetUserData());
				}
				else if (fixtureB->GetBody() == collision_body_ &&
						 fixtureB->GetFilterData().categoryBits == weaponCategoryBits())
				{
					b2CircleShape *shapeA = dynamic_cast<b2CircleShape *>(fixtureA->GetShape());
					b2CircleShape *shapeB = dynamic_cast<b2CircleShape *>(fixtureB->GetShape());
					targets.resize(targets.size() + 1);
					targets.back().collision_pos = getCollisionPosition(shapeB, shapeA, fixtureA->GetBody());
					targets.back().entity = reinterpret_cast<BaseGameEntity*>(fixtureA->GetBody()->GetUserData());
				}
			}
			contactEdge = contactEdge->next;
		}
	}
	return targets;
}

/** 父类函数重写 **/
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