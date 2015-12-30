#include "Boss.h"
#include "ShapeCategory.h"
#include "GameEntityConfig.h"
#include "State/BossOwnedStates.h"


Boss::Boss(std::shared_ptr<b2World> world)
	: BaseGameEntity(world)
{

}

Boss::~Boss()
{

}

bool Boss::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	setDirection(Left);

	state_machine_.reset(new StateMachine<Boss>(this));
	state_machine_->set_global_state(BossGlobal::instance());
	state_machine_->change_state(BossWalk::instance());

	setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
	setContentSize(cocos2d::Size(realWidth(), realHeight()));

	return true;
}

void Boss::update()
{
	state_machine_->update();
}

std::string Boss::name() const
{
	return "Boss";
}

int Boss::weaponCategoryBits() const
{
	return ShapeCategory::shape_enemy_weapon;
}

void Boss::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 获取实际宽度
float Boss::realWidth()
{
	EntityAttribute attribute;
	if (GameEntityConfig::instance()->getEntityAttribute("Boss", &attribute))
	{
		return attribute.width;
	}
	return 0;
}

// 获取实际高度
float Boss::realHeight()
{
	EntityAttribute attribute;
	if (GameEntityConfig::instance()->getEntityAttribute("Boss", &attribute))
	{
		return attribute.height;
	}
	return 0;
}

// 获取有限状态机
StateMachine<Boss>* Boss::getStateMachine()
{
	return state_machine_.get();
}