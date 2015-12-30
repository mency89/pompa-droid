#include "Hero.h"
#include "ShapeCategory.h"
#include "GameEntityConfig.h"
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
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
	state_machine_->update();
}

std::string Hero::name() const
{
	return "Hero";
}

int Hero::weaponCategoryBits() const
{
	return ShapeCategory::shape_hero_weapon;
}

void Hero::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 获取有限状态机
StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}