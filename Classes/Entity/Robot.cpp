#include "Robot.h"
#include "ShapeCategory.h"
#include "State/RobotOwnedStates.h"
using namespace cocos2d;


Robot::Robot(std::shared_ptr<b2World> world)
	: BaseGameEntity(world)
	, player_(nullptr)
{

}

Robot::~Robot()
{

}

bool Robot::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	player_ = Sprite::create();
	player_->setAnchorPoint(Vec2::ZERO);
	addChild(player_);

	state_machine_.reset(new StateMachine<Robot>(this));
	state_machine_->change_state(RobotIdle::instance());
	state_machine_->set_global_state(RobotGlobal::instance());

	return true;
}

void Robot::update()
{
	state_machine_->update();
}

const char* Robot::name() const
{
	return "robot";
}

int Robot::weaponCategoryBits() const
{
	return kEnemyWeaponShape;
}

void Robot::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 获取特效播放器
Node* Robot::getEffectsPlayer()
{
	player_->setFlippedX(isFlippedX());
	return player_;
}

// 获取有限状态机
StateMachine<Robot>* Robot::getStateMachine()
{
	return state_machine_.get();
}