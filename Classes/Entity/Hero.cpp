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
	setRunSpeed(getWalkSpeed() * 2);

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
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