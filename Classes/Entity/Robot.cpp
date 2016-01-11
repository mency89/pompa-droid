#include "Robot.h"
#include "ShapeCategory.h"
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

	state_machine_.reset(new StateMachine<Robot>(this));

	player_ = Sprite::create();
	player_->setAnchorPoint(Vec2::ZERO);
	addChild(player_);

	return true;
}

void Robot::update()
{
	state_machine_->update();
}

const char* Robot::name() const
{
	return "Robot";
}

int Robot::weaponCategoryBits() const
{
	return kEnemyWeaponShape;
}

void Robot::handleMenssage(const Message &msg)
{
	// ignore
}

// 获取特效播放器
Node* Robot::getEffectsPlayer()
{
	return player_;
}

// 获取有限状态机
StateMachine<Robot>* Robot::getStateMachine()
{
	return state_machine_.get();
}