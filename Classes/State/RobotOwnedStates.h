#ifndef __ROBOTOWNEDSTATES_H__
#define __ROBOTOWNEDSTATES_H__

#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Robot.h"

template <>
struct StateMachineData < Robot >
{
	typedef std::chrono::system_clock::time_point time_point;

	bool hit_hero;					// 击中英雄
	int hurt_source;				// 伤害来源
	int was_hit_count;				// 连续受击次数
	time_point was_hit_time;		// 上次受击时间
	time_point end_resting_time;	// 结束休息时间
	cocos2d::Vec2 target_pos;		// 目标位置
	StateMachineData()
		: hurt_source(0)
		, hit_hero(false)
		, was_hit_count(0)
	{
	}
};

/************************************************************************/
/* Base layer state                                                     */
/************************************************************************/

/**
 * 机器人待机状态
 */
class RobotIdle : public State < Robot >, public Singleton < RobotIdle >
{
	SINGLETON_DEFAULT(RobotIdle);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人行走状态
 */
class RobotWalk : public State < Robot >, public Singleton < RobotWalk >
{
	SINGLETON_DEFAULT(RobotWalk);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人攻击状态
 */
class RobotAttack : public State < Robot >, public Singleton < RobotAttack >
{
	SINGLETON_DEFAULT(RobotAttack);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人受击状态
 */
class RobotHurt : public State < Robot >, public Singleton < RobotHurt >
{
	SINGLETON_DEFAULT(RobotHurt);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人倒下状态
 */
class RobotKnockout : public State < Robot >, public Singleton < RobotKnockout >
{
	SINGLETON_DEFAULT(RobotKnockout);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人起身状态
 */
class RobotGetup : public State < Robot >, public Singleton < RobotGetup >
{
	SINGLETON_DEFAULT(RobotGetup);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/************************************************************************/
/* Behavior layer state                                                 */
/************************************************************************/

/**
 * 机器人直线行走状态
 */
class RobotBeelineWalk : public State < Robot >, public Singleton < RobotBeelineWalk >
{
	SINGLETON_DEFAULT(RobotBeelineWalk);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/**
 * 机器人休息状态
 */
class RobotIdleLittleWhile : public State < Robot >, public Singleton < RobotIdleLittleWhile >
{
	SINGLETON_DEFAULT(RobotIdleLittleWhile);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

/************************************************************************/
/* Role layer state											            */
/************************************************************************/

/**
 * Boss全局状态
 */
class RobotGlobal : public State < Robot >, public Singleton < RobotGlobal >
{
	SINGLETON_DEFAULT(RobotGlobal);

public:
	virtual void enter(Robot *object) override;

	virtual void exit(Robot *object) override;

	virtual void execute(Robot *object) override;

	virtual bool on_message(Robot *object, const Message &msg) override;
};

#endif