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

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
	state_machine_->update();
	auto f = Sprite::getSpriteFrame();
	if (f != nullptr)
	{
		CCLOG("%s", f->getPolygonInfo().filename.c_str());
	}
	
}

StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}