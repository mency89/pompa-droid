﻿#ifndef __TRASHCAN_H__
#define __TRASHCAN_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Trashcan : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Trashcan> > TrashcanStateMachine;

public:
	const static int kWidth3d = 27;

public:
	Trashcan(int type, std::shared_ptr<b2World> world);

	~Trashcan();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取有限状态机
	 */
	StateMachine<Trashcan>* getStateMachine();

private:
	TrashcanStateMachine state_machine_;
};

#endif