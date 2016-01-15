#include "Robot.h"
#include "ShapeCategory.h"
#include "State/RobotOwnedStates.h"
using namespace cocos2d;


Robot::Robot(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
	, belt_skin_(nullptr)
	, smoke_skin_(nullptr)
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

	belt_skin_ = Sprite::create();
	belt_skin_->setAnchorPoint(Vec2::ZERO);
	addChild(belt_skin_);

	smoke_skin_ = Sprite::create();
	smoke_skin_->setAnchorPoint(Vec2::ZERO);
	addChild(smoke_skin_);

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
Node* Robot::getBeltSkin()
{
	belt_skin_->setFlippedX(isFlippedX());
	return belt_skin_;
}

// 获取烟雾
Node* Robot::getSmokeSkin()
{
	smoke_skin_->setFlippedX(isFlippedX());
	return smoke_skin_;
}

// 获取有限状态机
StateMachine<Robot>* Robot::getStateMachine()
{
	return state_machine_.get();
}