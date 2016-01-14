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
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base_idle");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotIdle);
	object->runAction(animate);

	animation = AnimationManger::instance()->getAnimation("robot_smoke_idle");
	animation->setLoops(-1);
	animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotSmokeIdle);
	object->getEffectsPlayer()->runAction(animate);
}

void RobotIdle::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotIdle);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeIdle);
}

void RobotIdle::execute(Robot *object)
{
}

bool RobotIdle::on_message(Robot *object, const Message &msg)
{
	return false;
}

/******机器人行走状态******/

void RobotWalk::enter(Robot *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base_walk");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotWalk);
	object->runAction(animate);

	animation = AnimationManger::instance()->getAnimation("robot_smoke_walk");
	animation->setLoops(-1);
	animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotSmokeWalk);
	object->getEffectsPlayer()->runAction(animate);
}

void RobotWalk::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotWalk);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeWalk);
}

void RobotWalk::execute(Robot *object)
{
	object->moveEntity(object->getWalkSpeed());
}

bool RobotWalk::on_message(Robot *object, const Message &msg)
{
	return false;
}

/******机器人攻击状态******/

void RobotAttack::enter(Robot *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base_attack");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotAttack);
	object->runAction(animate);

	animation = AnimationManger::instance()->getAnimation("robot_smoke_attack");
	animation->setRestoreOriginalFrame(false);
	animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotSmokeAttack);
	object->getEffectsPlayer()->runAction(animate);
}

void RobotAttack::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotAttack);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeAttack);
	object->getStateMachine()->userdata().hit_hero = false;
}

void RobotAttack::execute(Robot *object)
{
	if (object->getActionByTag(ActionTags::kRobotAttack) == nullptr)
	{
		object->getStateMachine()->change_state(RobotIdle::instance());
	}
}

bool RobotAttack::on_message(Robot *object, const Message &msg)
{
	return false;
}

/******机器人受击状态******/

void RobotHurt::enter(Robot *object)
{
	system_clock::time_point current_time = system_clock::now();
	system_clock::time_point last_hurt_time = object->getStateMachine()->userdata().was_hit_time;
	system_clock::duration duration = current_time - last_hurt_time;
	if (duration_cast<milliseconds>(duration).count() < 1000)
	{
		++object->getStateMachine()->userdata().was_hit_count;
	}
	else
	{
		object->getStateMachine()->userdata().was_hit_count = 1;
	}

	if (object->getStateMachine()->userdata().was_hit_count < 3)
	{
		Animation *animation = AnimationManger::instance()->getAnimation("robot_base_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::kRobotHurt);
		object->runAction(animate);

		animation = AnimationManger::instance()->getAnimation("robot_smoke_hurt");
		animate = Animate::create(animation);
		animate->setTag(ActionTags::kRobotSmokeHurt);
		object->getEffectsPlayer()->runAction(animate);

		object->getStateMachine()->userdata().was_hit_time = system_clock::now();
	}
	else
	{
		object->getStateMachine()->change_state(RobotKnockout::instance());
	}
}

void RobotHurt::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotHurt);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeHurt);
}

void RobotHurt::execute(Robot *object)
{
	if (object->getActionByTag(ActionTags::kRobotHurt) == nullptr)
	{
		object->getStateMachine()->change_state(RobotIdle::instance());
	}
}

bool RobotHurt::on_message(Robot *object, const Message &msg)
{
	return false;
}

/******机器人倒下状态******/

void RobotKnockout::enter(Robot *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base_knockout");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotKnockout);
	object->runAction(animate);

	animation = AnimationManger::instance()->getAnimation("robot_smoke_knockout");
	animation->setRestoreOriginalFrame(false);
	animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotSmokeKnockout);
	object->getEffectsPlayer()->runAction(animate);
}

void RobotKnockout::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotKnockout);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeKnockout);
}

void RobotKnockout::execute(Robot *object)
{
	if (object->getActionByTag(ActionTags::kRobotKnockout) == nullptr)
	{
		if (object->isDeath())
		{
			object->getStateMachine()->set_current_state(nullptr);
			object->getStateMachine()->set_global_state(nullptr);
		}
		else
		{
			object->getStateMachine()->change_state(RobotGetup::instance());
		}
	}
}

bool RobotKnockout::on_message(Robot *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
}

/******Boss起身状态******/

void RobotGetup::enter(Robot *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("robot_base_getup");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotGetup);
	object->runAction(animate);

	animation = AnimationManger::instance()->getAnimation("robot_smoke_getup");
	animation->setRestoreOriginalFrame(false);
	animate = Animate::create(animation);
	animate->setTag(ActionTags::kRobotSmokeGetup);
	object->getEffectsPlayer()->runAction(animate);
}

void RobotGetup::exit(Robot *object)
{
	object->stopActionByTag(ActionTags::kRobotGetup);
	object->getEffectsPlayer()->stopActionByTag(ActionTags::kRobotSmokeGetup);
}

void RobotGetup::execute(Robot *object)
{
	if (object->getActionByTag(ActionTags::kRobotGetup) == nullptr)
	{
		// 面向对你造成伤害者
		int entity_id = object->getStateMachine()->userdata().hurt_source;
		BaseGameEntity *entity = object->getEntityManger()->getEntityByID(entity_id);
		if (entity != nullptr)
		{
			if (entity->getPositionX() < object->getPositionX())
			{
				object->setDirection(BaseGameEntity::kLeftDirection);
			}
			else
			{
				object->setDirection(BaseGameEntity::kRightDirection);
			}
		}
		object->getStateMachine()->change_state(RobotIdle::instance());
	}
}

bool RobotGetup::on_message(Robot *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
}

/************************************************************************/
/* Behavior layer state                                                 */
/************************************************************************/

/******机器人直线行走状态******/

void RobotBeelineWalk::enter(Robot *object)
{
	RobotWalk::instance()->enter(object);
}

void RobotBeelineWalk::exit(Robot *object)
{
	RobotWalk::instance()->exit(object);
}

void RobotBeelineWalk::execute(Robot *object)
{
	const cocos2d::Vec2 &target_pos = object->getStateMachine()->userdata().target_pos;
	if (target_pos.distance(object->getPosition()) <= object->getWalkSpeed())
	{
		object->setPosition(target_pos);
		object->getStateMachine()->change_state(RobotIdle::instance());
	}
	else
	{
		cocos2d::Vec2 velocity = target_pos - object->getPosition();
		velocity.normalize();
		velocity.x *= object->getWalkSpeed();
		velocity.y *= object->getWalkSpeed();
		object->moveEntity(velocity);
	}
}

bool RobotBeelineWalk::on_message(Robot *object, const Message &msg)
{
	return RobotWalk::instance()->on_message(object, msg);
}

/******机器人休息一会儿状态******/
void RobotIdleLittleWhile::enter(Robot *object)
{
	RobotIdle::instance()->enter(object);
}

void RobotIdleLittleWhile::exit(Robot *object)
{
	RobotIdle::instance()->exit(object);
}

void RobotIdleLittleWhile::execute(Robot *object)
{
	if (object->getStateMachine()->userdata().end_resting_time < system_clock::now())
	{
		object->getStateMachine()->change_state(RobotIdle::instance());
	}
}

bool RobotIdleLittleWhile::on_message(Robot *object, const Message &msg)
{
	return RobotIdle::instance()->on_message(object, msg);
}

/************************************************************************/
/* Role layer state											            */
/************************************************************************/

/******机器人全局状态******/

void RobotGlobal::enter(Robot *object)
{
}

void RobotGlobal::exit(Robot *object)
{
}

void RobotGlobal::execute(Robot *object)
{
	SimpleRobotLogic<Robot, RobotIdle, RobotIdleLittleWhile, RobotBeelineWalk, RobotAttack>(object);
}

bool RobotGlobal::on_message(Robot *object, const Message &msg)
{
	if (msg.msg_code == kMsgEntityHurt)
	{
		STEntityHurt extra_info = *reinterpret_cast<const STEntityHurt*>(msg.extra_info);
		object->getStateMachine()->userdata().hurt_source = msg.sender;
		object->getEntityManger()->getCurrentLevel()->playHitEffect(extra_info.local_pos, extra_info.value);
		object->hurtLife(extra_info.value);
		if (object->isDeath())
		{
			object->getStateMachine()->change_state(RobotKnockout::instance());
		}
		else
		{
			object->getStateMachine()->change_state(RobotHurt::instance());
		}	
		return true;
	}
	return false;
}