﻿#ifndef __HERO_H__
#define __HERO_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Hero : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Hero> > HeroStateMachine;

public:
	Hero(std::shared_ptr<b2World> world);

	~Hero();

	virtual bool init() override;

	virtual void update() override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Telegram &msg) override;

public:
	StateMachine<Hero>* getStateMachine();

private:
	HeroStateMachine state_machine_;
};

#endif