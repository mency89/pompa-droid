#ifndef __HERO_H__
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

	virtual std::string name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取有限状态机
	 */
	StateMachine<Hero>* getStateMachine();

private:
	HeroStateMachine state_machine_;
};

#endif