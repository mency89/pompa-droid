#include "BossOwnedStates.h"

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

/******Boss待机状态******/

void BossIdle::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_idle");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kBossIdle);
	object->runAction(animate);
}

void BossIdle::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossIdle);
}

void BossIdle::execute(Boss *object)
{
}

bool BossIdle::on_message(Boss *object, const Message &msg)
{
	return false;
}

/******Boss攻击状态******/

void BossAttack::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_attack");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kBossAttack);
	object->runAction(animate);
}

void BossAttack::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossAttack);
	object->getStateMachine()->userdata().hit_hero = false;
}

void BossAttack::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::kBossAttack) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossAttack::on_message(Boss *object, const Message &msg)
{
	return false;
}

/******Boss受击状态******/

void BossHurt::enter(Boss *object)
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

	if (object->getStateMachine()->userdata().was_hit_count < 5)
	{
		Animation *animation = AnimationManger::instance()->getAnimation("boss_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::kBossHurt);
		object->runAction(animate);
		object->getStateMachine()->userdata().was_hit_time = system_clock::now();
	}
	else
	{
		object->getStateMachine()->change_state(BossKnockout::instance());
	}
}

void BossHurt::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossHurt);
}

void BossHurt::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::kBossHurt) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossHurt::on_message(Boss *object, const Message &msg)
{
	return false;
}

/******Boss倒下状态******/

void BossKnockout::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_knockout");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kBossKnockout);
	object->runAction(animate);
}

void BossKnockout::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossKnockout);
}

void BossKnockout::execute(Boss *object)
{
	object->stepback(1.0f);

	if (object->getActionByTag(ActionTags::kBossKnockout) == nullptr)
	{
		if (object->isDeath())
		{
			object->getStateMachine()->set_current_state(nullptr);
			object->getStateMachine()->set_global_state(nullptr);
		}
		else
		{
			object->getStateMachine()->change_state(BossGetup::instance());
		}
	}
}

bool BossKnockout::on_message(Boss *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
}

/******Boss起身状态******/

void BossGetup::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_getup");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kBossGetup);
	object->runAction(animate);
}

void BossGetup::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossGetup);
}

void BossGetup::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::kBossGetup) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossGetup::on_message(Boss *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
}

/************************************************************************/
/* Behavior layer state                                                 */
/************************************************************************/

/******Boss行走状态******/

void BossWalk::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_walk");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kBossWalk);
	object->runAction(animate);
}

void BossWalk::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::kBossWalk);
}

void BossWalk::execute(Boss *object)
{
	const cocos2d::Vec2 &target_pos = object->getStateMachine()->userdata().target_pos;
	if (target_pos.distance(object->getPosition()) <= object->getWalkSpeed())
	{
		object->setPosition(target_pos);
		object->getStateMachine()->change_state(BossIdle::instance());
	}
	else
	{
		cocos2d::Vec2 velocity = target_pos - object->getPosition();
		velocity.normalize();
		velocity.x *= object->getWalkSpeed();
		velocity.y *= object->getWalkSpeed();
		object->move(velocity);
	}
}

bool BossWalk::on_message(Boss *object, const Message &msg)
{
	return false;
}

/******Boss休息一会儿状态******/
void BossIdleLittleWhile::enter(Boss *object)
{
	BossIdle::instance()->enter(object);
}

void BossIdleLittleWhile::exit(Boss *object)
{
	BossIdle::instance()->exit(object);
}

void BossIdleLittleWhile::execute(Boss *object)
{
	if (object->getStateMachine()->userdata().end_resting_time < system_clock::now())
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossIdleLittleWhile::on_message(Boss *object, const Message &msg)
{
	return BossIdle::instance()->on_message(object, msg);
}

/************************************************************************/
/* Role layer state											            */
/************************************************************************/

/******Boss全局状态******/

void BossGlobal::enter(Boss *object)
{
}

void BossGlobal::exit(Boss *object)
{
}

void BossGlobal::execute(Boss *object)
{
	SimpleRobotLogic<Boss, BossIdle, BossIdleLittleWhile, BossWalk, BossAttack>(object);
}

bool BossGlobal::on_message(Boss *object, const Message &msg)
{
	if (msg.msg_code == kMsgEntityHurt)
	{
		STEntityHurt extra_info = *reinterpret_cast<const STEntityHurt*>(msg.extra_info);
		object->getStateMachine()->userdata().hurt_source = msg.sender;
		object->getEntityManger()->getCurrentLevel()->playHitEffect(extra_info.local_pos, extra_info.value);
		object->hurtLife(extra_info.value);
		if (object->isDeath())
		{
			object->getStateMachine()->change_state(BossKnockout::instance());
		}
		else
		{
			object->getStateMachine()->change_state(BossHurt::instance());
		}
		return true;
	}
	return false;
}