#ifndef __HEROOWNEDSTATES_H__
#define __HEROOWNEDSTATES_H__

#include "State.h"
#include "Singleton.h"
#include "Entity/Hero.h"

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

#endif