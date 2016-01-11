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

#endif