#ifndef __BOSSOWNEDSTATES_H__
#define __BOSSOWNEDSTATES_H__

#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Boss.h"

template <>
struct StateMachineData < Boss >
{
	typedef std::chrono::system_clock::time_point time_point;

	int hurt_source;				// 伤害来源
	int continuous_hurt;			// 连续受击次数
	time_point last_hurt_time;		// 上次受击时间
	time_point end_resting_time;	// 结束休息时间
	cocos2d::Vec2 target_pos;		// 目标位置
	StateMachineData()
		: hurt_source(0)
		, continuous_hurt(0)
	{
	}
};

/************************************************************************/
/* Base layer state                                                     */
/************************************************************************/

/**
 * Boss待机状态
 */
class BossIdle : public State < Boss >, public Singleton < BossIdle >
{
	SINGLETON_DEFAULT(BossIdle);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/**
 * Boss行走状态
 */
class BossWalk : public State < Boss >, public Singleton < BossWalk >
{
	SINGLETON_DEFAULT(BossWalk);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/**
 * Boss受击状态
 */
class BossHurt : public State < Boss >, public Singleton < BossHurt >
{
	SINGLETON_DEFAULT(BossHurt);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/**
 * Boss倒下状态
 */
class BossKnockout : public State < Boss >, public Singleton < BossKnockout >
{
	SINGLETON_DEFAULT(BossKnockout);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/**
 * Boss起身状态
 */
class BossGetup : public State < Boss >, public Singleton < BossGetup >
{
	SINGLETON_DEFAULT(BossGetup);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/************************************************************************/
/* Behavior layer state                                                 */
/************************************************************************/

/**
 * Boss直线行走状态
 */
class BossBeelineWalk : public State < Boss >, public Singleton < BossBeelineWalk >
{
	SINGLETON_DEFAULT(BossBeelineWalk);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/**
 * Boss休息状态
 */
class BossIdelDelayTime : public State < Boss >, public Singleton < BossIdelDelayTime >
{
	SINGLETON_DEFAULT(BossIdelDelayTime);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

/************************************************************************/
/* Role layer state											            */
/************************************************************************/
/**
 * Boss全局状态
 */
class BossGlobal : public State < Boss >, public Singleton < BossGlobal >
{
	SINGLETON_DEFAULT(BossGlobal);

public:
	virtual void enter(Boss *object) override;

	virtual void exit(Boss *object) override;

	virtual void execute(Boss *object) override;

	virtual bool on_message(Boss *object, const Message &msg) override;
};

#endif