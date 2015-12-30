#ifndef __BOSS_H__
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

	virtual std::string name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取有限状态机
	 */
	StateMachine<Boss>* getStateMachine();

private:
	HeroStateMachine state_machine_;
};

#endif