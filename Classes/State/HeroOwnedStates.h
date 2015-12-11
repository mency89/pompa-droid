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
};

#endif