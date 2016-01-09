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

	state_machine_.reset(new StateMachine<Boss>(this));
	state_machine_->set_global_state(BossGlobal::instance());
	state_machine_->change_state(BossIdle::instance());

	return true;
}

void Boss::update()
{
	state_machine_->update();
}

const char* Boss::name() const
{
	return "Boss";
}

int Boss::weaponCategoryBits() const
{
	return kEnemyWeaponShape;
}

void Boss::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 获取有限状态机
StateMachine<Boss>* Boss::getStateMachine()
{
	return state_machine_.get();
}