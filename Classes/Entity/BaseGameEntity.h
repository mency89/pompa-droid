#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <memory>
#include "cocos2d.h"
#include "message/Telegram.h"
#include "GameEntityCreator.h"

class b2Body;
class b2World;
class b2CircleShape;
class EntityManger;

class BaseGameEntity : public cocos2d::Sprite
{
	friend class EntityManger;

public:
	enum Direction
	{
		Up,
		Down,
		Left,
		Right
	};

	struct Collision
	{
		BaseGameEntity* entity;
		cocos2d::Vec2 collision_pos;
		Collision()
			: entity(nullptr)
		{
		}
	};

public:
	BaseGameEntity(std::shared_ptr<b2World> world);

	virtual ~BaseGameEntity();

public:
	virtual bool init() override;

	virtual void setParent(cocos2d::Node* parent) override;

public:
	/**
	 * 每帧更新
	 */
	virtual void update();

	/**
	 * 武器位标识
	 */
	virtual int weaponCategoryBits() const = 0;

	/**
	 * 受击
	 * @param cocos2d::Vec2 受击位置
	 */
	virtual void onHurt(const cocos2d::Vec2 &pos);

	/**
	 * 处理消息
	 */
	virtual void handleMenssage(const Telegram &msg);

public:
	/**
	 * 获取唯一id
	 */
	int getID() const;

	/**
	 * 获取刚体
	 */
	b2Body* getBody();

	/**
	 * 获取管理器
	 */
	EntityManger* getEntityManger();

	/**
	 * 销毁刚体
	 */
	void destroyBody();

	/**
	 * 移动实体
	 * @param float 移动速度
	 */
	void moveEntity(float speed);

	/**
	 * 获取跑动速度
	 */
	float getRunSpeed() const;

	/**
	 * 设置跑动速度
	 */
	void setRunSpeed(float speed);

	/**
	 * 获取行走速度
	 */
	float getWalkSpeed() const;

	/**
	 * 设置行走速度
	 */
	void setWalkSpeed(float speed);

	/**
	 * 获取跳跃力
	 */
	float getJumpForce() const;

	/**
	 * 设置跳跃力
	 */
	void setJumpForce(float force);

	/**
	 * 获取最大跳跃高度
	 */
	float getMaxJumpHeight() const;

	/**
	 * 设置跳跃高度
	 */
	void setMaxJumpHeight(float height);

	/**
	 * 获取方向
	 */
	Direction getDirection() const;

	/**
	 * 获取面对的方向
	 */
	Direction getLookDirection() const;

	/**
	 * 设置方向
	 */
	void setDirection(Direction direction);

	/**
	 * 获取命中的目标
	 */
	std::vector<Collision> getHitTargets() const;

	/**
	 * 更新碰撞体
	 */
	void updateCollisionBodyBySpriteframe();

	/* 重写父类函数 */
public:
	virtual void setPositionX(float x) override;

	virtual void setPositionY(float y) override;

	virtual void setPosition(const cocos2d::Vec2 &pos) override;

	virtual void setPosition(float x, float y) override;

protected:
	/**
	 * 更新刚体位置
	 */
	void updateBodyPosition();

private:
	/**
	 * 设置管理器
	 */
	void setEntityManger(EntityManger *manager);
	
	/**
	 * 获取碰撞位置
	 */
	cocos2d::Vec2 getCollisionPosition(b2CircleShape *source, b2CircleShape *target, b2Body *target_body) const;

private:
	int							entity_id_;
	std::shared_ptr<b2World>	world_;
	EntityManger*				manager_;
	b2Body*						collision_body_;
	float						run_speed_;
	float						walk_speed_;
	float						jump_force_;
	float						jump_height_;
	Direction					direction_;
	int							hit_point_;
	std::string					shape_name_;
	static int					s_next_entity_id_;
};

#endif