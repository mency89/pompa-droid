#include "BaseGameEntity.h"

#include <limits>
#include <Box2D/Box2D.h>

#include "Helper.h"
#include "GB2ShapeCache.h"
#include "GameEntityConfig.h"
#include "Message/MessageDispatcher.h"
using namespace cocos2d;

int BaseGameEntity::s_next_entity_id_ = 0;

BaseGameEntity::BaseGameEntity(int type, std::shared_ptr<b2World> world)
	: type_(type)
	, world_(world)
	, hit_point_(0)
	, attack_(0)
	, run_attack_(0)
	, jump_attack_(0)
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

	// 创建刚体
	b2BodyDef def;
	def.userData = this;
	def.allowSleep = false;
	def.type = b2_dynamicBody;
	collision_body_ = world_->CreateBody(&def);

	// 设置方向
	setDirection(kRightDirection);

	// 设置属性
	const EntityAttribute *attribute = nullptr;
	if (GameEntityConfig::instance()->getEntityAttribute(name(), attribute))
	{
		setWalkSpeed(attribute->walk_speed);
		setRunSpeed(attribute->run_speed);
		setJumpForce(attribute->jump_force);
		setMaxJumpHeight(attribute->max_jump_height);
		setHitPoint(attribute->max_hit_point);
		setAttack(attribute->attack);
		setRunAttack(attribute->run_attack);
		setJumpAttack(attribute->jump_attack);
	}

	// 注册消息事件
	MessageDispatcher::instance()->registerEntity(this);

	return true;
}

void BaseGameEntity::setParent(cocos2d::Node* parent)
{
	Node::setParent(parent);
	updateBodyPosition();
}

// 每帧更新
void BaseGameEntity::update()
{
}

// 处理消息
void BaseGameEntity::handleMenssage(const Message &msg)
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
		shape_name_ = Splitext(getSpriteFrame()->getPolygonInfo().filename)[0];
		if (!shape_name_.empty())
		{
			// 添加身体形状
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape_name_, isFlippedX());
			// 添加武器形状
			GB2ShapeCache::instance()->addFixturesToBody(collision_body_, shape_name_ + "_weapon", isFlippedX());
			// 更新位置
			updateBodyPosition();
		}
	}
	else
	{
		shape_name_.clear();
	}
}

// 获取上一位置
float BaseGameEntity::getPreviousPositionX() const
{
	return previous_pos_.x;
}

float BaseGameEntity::getPreviousPositionY() const
{
	return previous_pos_.y;
}

cocos2d::Vec2 BaseGameEntity::getPreviousPosition() const
{
	return previous_pos_;
}

// 全宽
float BaseGameEntity::fullWidth() const
{
	const EntityAttribute *attribute = nullptr;
	if (GameEntityConfig::instance()->getEntityAttribute(name(), attribute))
	{
		return attribute->size.width;
	}
	return 0;
}

// 全高
float BaseGameEntity::fullHeight() const
{
	const EntityAttribute *attribute = nullptr;
	if (GameEntityConfig::instance()->getEntityAttribute(name(), attribute))
	{
		return attribute->size.height;
	}
	return 0;
}

// 实际宽度
float BaseGameEntity::realWidth() const
{
	const EntityAttribute *attribute = nullptr;
	if (GameEntityConfig::instance()->getEntityAttribute(name(), attribute))
	{
		return attribute->real_size.width;
	}
	return 0;
}

// 实际高度
float BaseGameEntity::realHeight() const
{
	const EntityAttribute *attribute = nullptr;
	if (GameEntityConfig::instance()->getEntityAttribute(name(), attribute))
	{
		return attribute->real_size.height;
	}
	return 0;
}

// 获取实际矩形框
Rect BaseGameEntity::getRealRect() const
{
	const Size size(fullWidth(), fullHeight());
	const Size real_size(realWidth(), realHeight());
	Vec2 origin = getPosition() - Vec2(size.width * getAnchorPoint().x, size.height * getAnchorPoint().y);
	Vec2 real_origin = origin + Vec2(size.width / 2 - real_size.width / 2, size.height / 2 - real_size.height / 2);
	Rect ret(real_origin.x, real_origin.y, real_size.width, real_size.height);
	return ret;
}

// 获取唯一id
int BaseGameEntity::getID() const
{
	return entity_id_;
}

// 获取类型
int BaseGameEntity::getType() const
{
	return type_;
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

// 获取攻击力
unsigned int BaseGameEntity::getAttack() const
{
	return attack_;
}

// 设置攻击力
void BaseGameEntity::setAttack(unsigned int value)
{
	attack_ = value;
}

// 获取奔跑攻击力
unsigned int BaseGameEntity::getRunAttack() const
{
	return run_attack_;
}

// 设置奔跑攻击力
void BaseGameEntity::setRunAttack(unsigned int value)
{
	run_attack_ = value;
}

// 获取跳跃攻击力
unsigned int BaseGameEntity::getJumpAttack() const
{
	return jump_attack_;
}

// 设置跳跃攻击力
void BaseGameEntity::setJumpAttack(unsigned int value)
{
	jump_attack_ = value;
}

// 获取血量
unsigned int BaseGameEntity::getHitPoint() const
{
	return hit_point_;
}

// 设置血量
void BaseGameEntity::setHitPoint(unsigned int value)
{
	hit_point_ = value;
}

// 伤害生命
void BaseGameEntity::hurtLife(unsigned int value)
{
	hit_point_ = value >= hit_point_ ? 0 : hit_point_ - value;
	if (isDeath() && collision_body_ != nullptr)
	{
		// 销毁所有形状
		b2Fixture *fixture = collision_body_->GetFixtureList();
		while (fixture != nullptr)
		{
			b2Fixture *next_fixture = fixture->GetNext();
			collision_body_->DestroyFixture(fixture);
			fixture = next_fixture;
		}
	}
}

// 是否死亡
bool BaseGameEntity::isDeath() const
{
	return hit_point_ == 0;
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

// 移动
void BaseGameEntity::move(float speed)
{
	if (direction_ == kUpDirection)
	{
		setPosition(getPosition() + Vec2(0, speed));
	}
	else if (direction_ == kDownDirection)
	{
		setPosition(getPosition() - Vec2(0, speed));
	}
	else if (direction_ == kLeftDirection)
	{
		setPosition(getPosition() - Vec2(speed, 0));
	}
	else
	{
		setPosition(getPosition() + Vec2(speed, 0));
	}
}

// 移动
void BaseGameEntity::move(const cocos2d::Vec2 &velocity)
{
	if (velocity.x > 0.0f)
	{
		setDirection(kRightDirection);
	}
	else if (velocity.x < 0.0f)
	{
		setDirection(kLeftDirection);
	}
	setPosition(getPosition() + velocity);
}

// 后退
void BaseGameEntity::stepback(float speed)
{
	setPositionX(getPositionX() + (isFlippedX() ? speed : -speed));
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
	return isFlippedX() ? kLeftDirection : kRightDirection;
}

// 设置方向
void BaseGameEntity::setDirection(Direction direction)
{
	direction_ = direction;
	if (direction_ == kRightDirection)
	{
		setFlippedX(false);
	}
	else if (direction_ == kLeftDirection)
	{
		setFlippedX(true);
	}
}

// 更新刚体位置
void BaseGameEntity::updateBodyPosition()
{
	if (collision_body_ != nullptr && getParent() != nullptr)
	{
		Vec2 shape_anchor;
		const float PTMRatio = GB2ShapeCache::instance()->getPTMRatio();
		Vec2 world_pos = getParent()->convertToWorldSpace(getPosition());
		if (!shape_name_.empty() && GB2ShapeCache::instance()->anchorPointForShape(shape_name_, shape_anchor))
		{
			const Size size = getContentSize();
			Vec2 origin = world_pos - Vec2(getAnchorPoint().x * size.width, getAnchorPoint().y * size.height);
			world_pos.x = origin.x + shape_anchor.x * size.width;
			world_pos.y = origin.y + shape_anchor.y * size.height;
		}
		collision_body_->SetTransform(b2Vec2(world_pos.x / PTMRatio, world_pos.y / PTMRatio), collision_body_->GetAngle());
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
}

void BaseGameEntity::setPositionY(float y)
{
	Sprite::setPositionY(y);
}

void BaseGameEntity::setPosition(const Vec2 &pos)
{
	Sprite::setPosition(pos);
}

void BaseGameEntity::setPosition(float x, float y)
{
	previous_pos_ = getPosition();
	Sprite::setPosition(x, y);;
	updateBodyPosition();
}