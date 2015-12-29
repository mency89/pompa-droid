#include "Hero.h"
#include "ShapeCategory.h"
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
	setJumpForce(2.0f);
	setMaxJumpHeight(50);

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
	setContentSize(cocos2d::Size(RealWidth, RealHeight));

	return true;
}

void Hero::update()
{
	state_machine_->update();
}

int Hero::weaponCategoryBits() const
{
	return ShapeCategory::shape_hero_weapon;
}

void Hero::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}