#include "Robot.h"
#include "ShapeCategory.h"
#include "State/RobotOwnedStates.h"
using namespace cocos2d;


Robot::Robot(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
	, belt_(nullptr)
	, smoke_(nullptr)
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

	belt_ = Sprite::create();
	belt_->setAnchorPoint(Vec2::ZERO);
	addChild(belt_);

	smoke_ = Sprite::create();
	smoke_->setAnchorPoint(Vec2::ZERO);
	addChild(smoke_);

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

// 获取腰带
Node* Robot::getBelt()
{
	belt_->setFlippedX(isFlippedX());
	return belt_;
}

// 获取烟雾
Node* Robot::getSmoke()
{
	smoke_->setFlippedX(isFlippedX());
	return smoke_;
}

// 获取有限状态机
StateMachine<Robot>* Robot::getStateMachine()
{
	return state_machine_.get();
}