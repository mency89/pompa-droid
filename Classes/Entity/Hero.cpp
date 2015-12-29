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

	EntityAttribute attribute;
	if (GameEntityConfig::instance()->getEntityAttribute("Hero", &attribute))
	{
		setWalkSpeed(attribute.walk_speed);
		setRunSpeed(attribute.run_speed);
		setJumpForce(attribute.jump_force);
		setMaxJumpHeight(attribute.max_jump_height);
	}

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
	setContentSize(cocos2d::Size(realWidth(), realHeight()));

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

// 获取实际宽度
float Hero::realWidth()
{
	EntityAttribute attribute;
	if (GameEntityConfig::instance()->getEntityAttribute("Hero", &attribute))
	{
		return attribute.width;
	}
	return 0;
}

// 获取实际高度
float Hero::realHeight()
{
	EntityAttribute attribute;
	if (GameEntityConfig::instance()->getEntityAttribute("Hero", &attribute))
	{
		return attribute.height;
	}
	return 0;
}

// 获取有限状态机
StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}