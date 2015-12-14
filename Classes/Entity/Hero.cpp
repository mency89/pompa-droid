#include "Hero.h"
#include "State/HeroOwnedStates.h"


Hero::Hero(std::shared_ptr<b2World> world)
	: BaseGameEntity(world)
{

}

Hero::~Hero()
{

}

bool Hero::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	setWalkSpeed(1.0f);
	setRunSpeed(getWalkSpeed() * 3);
	setJumpForce(2.5f);
	setMaxJumpHeight(50);

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
	BaseGameEntity::update();
	state_machine_->update();
}

void Hero::handleMenssage(const Telegram &msg)
{
	state_machine_->handle_message(msg);
}

StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}