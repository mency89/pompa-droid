﻿#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Weapon : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Weapon> > WeaponStateMachine;

public:
	Weapon(int type, std::shared_ptr<b2World> world);

	~Weapon();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取有限状态机
	 */
	StateMachine<Weapon>* getStateMachine();

private:
	WeaponStateMachine state_machine_;
};

#endif