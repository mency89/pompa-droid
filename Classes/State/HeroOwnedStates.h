#ifndef __HEROOWNEDSTATES_H__
#define __HEROOWNEDSTATES_H__

#include <set>
#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Hero.h"

template <>
struct StatePrivateData < Hero >
{
	typedef std::chrono::system_clock::time_point time_point;

	bool jump_up;								// 是否处于上跳状态
	float before_jump_y;						// 跳跃前y值
	int hurt_source;							// 伤害来源
	int was_hit_count;							// 被连续攻击次数
	int hit_target_count;						// 连击目标次数
	time_point was_hit_time;					// 上次受击时间
	time_point hit_target_time;					// 上次击中目标的时间
	std::set<int> hit_targets;					// 击中的目标集合
	time_point last_direction_key_pressed_time;	// 上次按下方向键的时间
	StatePrivateData()
		: jump_up(false)
		, hurt_source(0)
		, was_hit_count(0)
		, hit_target_count(0)
		, before_jump_y(0.0f)
	{
	}
};

/**
 * 英雄待机状态
 */
class HeroIdle : public State < Hero >, public Singleton < HeroIdle >
{
	SINGLETON_DEFAULT(HeroIdle);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄行走状态
 */
class HeroWalk : public State < Hero >, public Singleton < HeroWalk >
{
	SINGLETON_DEFAULT(HeroWalk);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄奔跑状态
 */
class HeroRun : public State < Hero >, public Singleton < HeroRun >
{
	SINGLETON_DEFAULT(HeroRun);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄跳跃状态
 */
class HeroJump : public State < Hero >, public Singleton < HeroJump >
{
	SINGLETON_DEFAULT(HeroJump);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄攻击状态
 */
class HeroAttack : public State < Hero >, public Singleton < HeroAttack >
{
	SINGLETON_DEFAULT(HeroAttack);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄奔跑攻击状态
 */
class HeroRuningAttack : public State < Hero >, public Singleton < HeroRuningAttack >
{
	SINGLETON_DEFAULT(HeroRuningAttack);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄跳跃攻击状态
 */
class HeroJumpingAttack : public State < Hero >, public Singleton < HeroJumpingAttack >
{
	SINGLETON_DEFAULT(HeroJumpingAttack);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄受击状态
 */
class HeroHurt : public State < Hero >, public Singleton < HeroHurt >
{
	SINGLETON_DEFAULT(HeroHurt);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄倒下状态
 */
class HeroKnockout : public State < Hero >, public Singleton < HeroKnockout >
{
	SINGLETON_DEFAULT(HeroKnockout);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄起身状态
 */
class HeroGetup : public State < Hero >, public Singleton < HeroGetup >
{
	SINGLETON_DEFAULT(HeroGetup);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

/**
 * 英雄全局状态
 */
class HeroGlobal : public State < Hero >, public Singleton < HeroGlobal >
{
	SINGLETON_DEFAULT(HeroGlobal);

public:
	virtual void enter(Hero *object) override;

	virtual void exit(Hero *object) override;

	virtual void execute(Hero *object) override;

	virtual bool on_message(Hero *object, const Message &msg) override;
};

#endif