#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <memory>
#include "cocos2d.h"
#include "Telegram.h"
#include "GameEntityCreator.h"

class b2Body;
class b2World;

class BaseGameEntity : public cocos2d::Sprite
{
public:
	enum Direction
	{
		Up,
		Down,
		Left,
		Right
	};

public:
	BaseGameEntity(std::shared_ptr<b2World> world);

	virtual ~BaseGameEntity();

	virtual bool init() override;

	virtual void update();

	virtual void handleMenssage(const Telegram &msg);

	void update_collision_body_by_spriteframe();

public:
	/**
	 * 获取唯一id
	 */
	int get_id() const;

	/**
	 * 移动
	 * @param float 移动速度
	 */
	void move(float speed);

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
	 * 设置方向
	 */
	void setDirection(Direction direction);

private:
	int							entity_id_;
	std::shared_ptr<b2World>	world_;
	b2Body*						collision_body_;
	float						run_speed_;
	float						walk_speed_;
	float						jump_force_;
	float						jump_height_;
	Direction					direction_;
	std::string					sprite_frame_name_;
	static int					s_next_entity_id_;
};

#endif