#include "Boss.h"
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

	setWalkSpeed(1.0f);
	setRunSpeed(getWalkSpeed() * 3);
	setJumpForce(2.5f);
	setMaxJumpHeight(50);
	setDirection(Left);

	state_machine_.reset(new StateMachine<Boss>(this));
	state_machine_->set_global_state(BossGlobal::instance());
	state_machine_->change_state(BossIdle::instance());

	return true;
}

void Boss::update()
{
	state_machine_->update();
}

void Boss::handleMenssage(const Telegram &msg)
{
	state_machine_->handle_message(msg);
}

StateMachine<Boss>* Boss::getStateMachine()
{
	return state_machine_.get();
}