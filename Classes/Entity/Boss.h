﻿#ifndef __BOSS_H__
#define __BOSS_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Boss : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Boss> > HeroStateMachine;

public:
	Boss(std::shared_ptr<b2World> world);

	~Boss();

	virtual bool init() override;

	virtual void update() override;

	virtual void handleMenssage(const Telegram &msg) override;

public:
	StateMachine<Boss>* getStateMachine();

private:
	HeroStateMachine state_machine_;
};

#endif