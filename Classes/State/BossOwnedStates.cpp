#include "BossOwnedStates.h"

#include "ActionTags.h"
#include "MeesageTypes.h"
#include "AnimationManger.h"
#include "MessageDispatcher.h"
#include "Entity/EntityManger.h"
using namespace cocos2d;


/******Boss待机状态******/

void BossIdle::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_idle");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::boss_idle);
	object->runAction(animate);
}

void BossIdle::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_idle);
}

void BossIdle::execute(Boss *object)
{
}

bool BossIdle::on_message(Boss *object, const Telegram &msg)
{
	return false;
}

/******Boss行走状态******/

void BossWalk::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_walk");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::boss_walk);
	object->runAction(animate);
}

void BossWalk::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_walk);
}

void BossWalk::execute(Boss *object)
{
	object->moveEntity(object->getWalkSpeed());
	if (object->getActionByTag(ActionTags::boss_walk) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossWalk::on_message(Boss *object, const Telegram &msg)
{
	return false;
}

/******Boss受击状态******/

void BossHurt::enter(Boss *object)
{
	std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point last_hurt_time = object->getStateMachine()->userdata().last_hurt_time;
	std::chrono::system_clock::duration duration = current_time - last_hurt_time;
	if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() < 1000)
	{
		++object->getStateMachine()->userdata().continuous_hurt;
	}
	else
	{
		object->getStateMachine()->userdata().continuous_hurt = 1;
	}

	if (object->getStateMachine()->userdata().continuous_hurt < 3)
	{
		Animation *animation = AnimationManger::instance()->getAnimation("boss_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::boss_hurt);
		object->runAction(animate);
		object->getStateMachine()->userdata().last_hurt_time = std::chrono::system_clock::now();
	}
	else
	{
		object->getStateMachine()->change_state(BossKnockout::instance());
	}
}

void BossHurt::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_hurt);
}

void BossHurt::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::boss_hurt) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossHurt::on_message(Boss *object, const Telegram &msg)
{
	return false;
}

/******Boss倒下状态******/

void BossKnockout::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_knockout");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::boss_knockout);
	object->runAction(animate);
}

void BossKnockout::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_knockout);
}

void BossKnockout::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::boss_knockout) == nullptr)
	{
		// 面向对你造成伤害者
		int entity_id = object->getStateMachine()->userdata().hurt_source;
		BaseGameEntity *entity = object->getEntityManger()->getEntityByID(entity_id);
		if (entity != nullptr)
		{
			if (entity->getPositionX() < object->getPositionX())
			{
				object->setDirection(BaseGameEntity::Left);
			}
			else
			{
				object->setDirection(BaseGameEntity::Right);
			}
		}
		object->getStateMachine()->change_state(BossGetup::instance());
	}
}

bool BossKnockout::on_message(Boss *object, const Telegram &msg)
{
	// 吞噬受击消息
	return msg.msg_code == msg_EntityHurt;
}

/******Boss起身状态******/

void BossGetup::enter(Boss *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("boss_getup");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::boss_getup);
	object->runAction(animate);
}

void BossGetup::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_getup);
}

void BossGetup::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::boss_getup) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
	}
}

bool BossGetup::on_message(Boss *object, const Telegram &msg)
{
	// 吞噬受击消息
	return msg.msg_code == msg_EntityHurt;
}

/******Boss全局状态******/

void BossGlobal::enter(Boss *object)
{
}

void BossGlobal::exit(Boss *object)
{
}

void BossGlobal::execute(Boss *object)
{
}

bool BossGlobal::on_message(Boss *object, const Telegram &msg)
{
	if (msg.msg_code == msg_EntityHurt)
	{
		MSEntityHurt extra_info = *reinterpret_cast<const MSEntityHurt*>(msg.extra_info);
		object->getStateMachine()->userdata().hurt_source = msg.sender;
		object->onHurt(extra_info.pos);
		object->getStateMachine()->change_state(BossHurt::instance());
		return true;
	}
	return false;
}