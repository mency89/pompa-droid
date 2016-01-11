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

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
	state_machine_->update();
}

const char* Hero::name() const
{
	return "hero";
}

int Hero::weaponCategoryBits() const
{
	return kHeroWeaponShape;
}

void Hero::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 是否是跳跃状态
bool Hero::isJumpingState() const
{
	return state_machine_->get_current_state() == HeroJump::instance() ||
		state_machine_->get_current_state() == HeroJumpingAttack::instance();
}

// 获取有限状态机
StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}