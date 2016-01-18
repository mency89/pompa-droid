#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <memory>
#include "cocos2d.h"
#include "Message/Message.h"
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
		kUpDirection = 1,
		kDownDirection = 2,
		kLeftDirection = 3,
		kRightDirection = 4
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
	BaseGameEntity(int type, std::shared_ptr<b2World> world);

	virtual ~BaseGameEntity();

public:
	virtual bool init() override;

	/**
	 * 每帧更新
	 */
	virtual void update();

	/**
	 * 实例名称
	 */
	virtual const char* name() const = 0;

	/**
	 * 武器位标识
	 */
	virtual int weaponCategoryBits() const = 0;

	/**
	 * 处理消息
	 */
	virtual void handleMenssage(const Message &msg);

public:
	/**
	 * 唯一id
	 */
	int getID() const;

	/**
	 * 获取类型
	 */
	int getType() const;

	/**
	 * 全宽
	 */
	float fullWidth() const;

	/**
	 * 全高
	 */
	float fullHeight() const;

	/**
	 * 实际宽度
	 */
	float realWidth() const;

	/**
	 * 实际高度
	 */
	float realHeight() const;

	/**
	 * 获取矩形框实际大小
	 */
	cocos2d::Rect getRealRect() const;

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
	 * 移动
	 * @param float 移动速度
	 */
	void move(float speed);

	/**
	 * 移动
	 */
	void move(const cocos2d::Vec2 &velocity);

	/**
	 * 后退
	 * @param float 移动速度
	 */
	void stepback(float speed);

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
	 * 获取攻击力
	 */
	virtual unsigned int getAttack() const;

	/**
	 * 获取奔跑攻击力
	 */
	virtual unsigned int getRunAttack() const;

	/**
	 * 获取跳跃攻击力
	 */
	virtual unsigned int getJumpAttack() const;

	/**
	 * 获取血量
	 */
	unsigned int getHitPoint() const;

	/**
	 * 伤害生命
	 */
	void hurtLife(unsigned int value);

	/**
	 * 是否死亡
	 */
	bool isDeath() const;

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

	/**
	 * 获取上一位置
	 */
	float getPreviousPositionX() const;
	float getPreviousPositionY() const;
	const cocos2d::Vec2& getPreviousPosition() const;


	/* 重写父类函数 */
public:
	virtual void setPositionX(float x) override;

	virtual void setPositionY(float y) override;

	virtual void setPosition(const cocos2d::Vec2 &pos) override;

	//virtual void setPosition(float x, float y) override;

	virtual void setParent(cocos2d::Node* parent) override;

protected:
	/**
	 * 更新刚体位置
	 */
	void updateBodyPosition();

private:
	/**
	 * 设置攻击力
	 */
	void setAttack(unsigned int value);

	/**
	 * 设置奔跑攻击力
	 */
	void setRunAttack(unsigned int value);

	/**
	 * 设置跳跃攻击力
	 */
	void setJumpAttack(unsigned int value);

	/**
	 * 设置血量
	 */
	void setHitPoint(unsigned int value);

	/**
	 * 设置管理器
	 */
	void setEntityManger(EntityManger *manager);
	
	/**
	 * 获取碰撞位置
	 */
	cocos2d::Vec2 getCollisionPosition(b2CircleShape *source, b2CircleShape *target, b2Body *target_body) const;

private:
	const int					type_;
	const int					entity_id_;
	std::shared_ptr<b2World>	world_;
	EntityManger*				manager_;
	b2Body*						collision_body_;
	cocos2d::Vec2				previous_pos_;
	float						run_speed_;
	float						walk_speed_;
	float						jump_force_;
	float						jump_height_;
	Direction					direction_;
	unsigned int				hit_point_;
	unsigned int				attack_;
	unsigned int				run_attack_;
	unsigned int				jump_attack_;
	std::string					shape_name_;
	static int					s_next_entity_id_;
};

#endif