#ifndef __HEROOWNEDSTATES_H__
#define __HEROOWNEDSTATES_H__

#include <set>
#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Hero.h"

template <>
struct StateMachineData < Hero >
{
	bool jump_up;							// 是否处于上跳状态
	float before_he_height;					// 跳跃前y值
	int hurt_source;						// 伤害来源
	int continuous_hurt;					// 被连续攻击次数
	std::set<int> hit_targets;				// 击中的目标集合
	std::chrono::system_clock::time_point last_direction_key_pressed_time;
	StateMachineData()
		: jump_up(false)
		, hurt_source(0)
		, continuous_hurt(0)
		, before_he_height(0.0f)
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
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

	virtual bool on_message(Hero *object, const Telegram &msg) override;
};

#endif