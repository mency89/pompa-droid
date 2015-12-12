#ifndef __HEROOWNEDSTATES_H__
#define __HEROOWNEDSTATES_H__

#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Hero.h"

template <>
struct StateMachineData < Hero >
{
	bool jump_up;
	float before_he_height;
	std::chrono::system_clock::time_point direction_key_pressed_time;

	StateMachineData()
		: jump_up(false)
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

#endif