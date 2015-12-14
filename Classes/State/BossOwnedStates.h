#ifndef __BOSSOWNEDSTATES_H__
#define __BOSSOWNEDSTATES_H__

#include <chrono>
#include "State.h"
#include "Singleton.h"
#include "Entity/Boss.h"

template <>
struct StateMachineData < Boss >
{
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