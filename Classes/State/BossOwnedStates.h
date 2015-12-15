#ifndef __BOSSOWNEDSTATES_H__
#define __BOSSOWNEDSTATES_H__

#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Boss.h"

template <>
struct StateMachineData < Boss >
{
	int continuous_hurt;				// 连续受击次数

	StateMachineData()
		: continuous_hurt(0)
	{
	}
};

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

	virtual bool on_message(Boss *object, const Telegram &msg) override;
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

	virtual bool on_message(Boss *object, const Telegram &msg) override;
};

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

	virtual bool on_message(Boss *object, const Telegram &msg) override;
};

#endif