#include "RobotOwnedStates.h"

#include "ActionTags.h"
#include "AnimationManger.h"
#include "Scene/LevelLayer.h"
#include "SimpleRobotLogic.h"
#include "Entity/Hero.h"
#include "Entity/EntityManger.h"
#include "Message/MeesageTypes.h"
#include "Message/MessageDispatcher.h"
using namespace cocos2d;
using namespace std::chrono;


/************************************************************************/
/* Base layer state                                                     */
/************************************************************************/

/******机器人待机状态******/

void RobotIdle::enter(Robot *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotIdle);
	object->runAction(animate);
}

void RobotIdle::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotIdle);
}

void RobotIdle::execute(Robot *object)
{
}

bool RobotIdle::on_message(Robot *object, const Message &msg)
{
	return false;
}