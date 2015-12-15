﻿#include "BossOwnedStates.h"

#include "ActionTags.h"
#include "MeesageTypes.h"
#include "AnimationManger.h"
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
	Sequence *action = Sequence::create(animate, DelayTime::create(0.1f), nullptr);
	action->setTag(ActionTags::boss_knockout);
	object->runAction(action);
}

void BossKnockout::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_knockout);
}

void BossKnockout::execute(Boss *object)
{
	if (object->getActionByTag(ActionTags::boss_knockout) == nullptr)
	{
		object->getStateMachine()->change_state(BossGetup::instance());
	}
}

bool BossKnockout::on_message(Boss *object, const Telegram &msg)
{
	// 吞噬受击消息
	return msg.msg_code == msg_Hurt;
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
	return msg.msg_code == msg_Hurt;
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
	if (msg.msg_code == msg_Hurt)
	{
		object->getStateMachine()->change_state(BossHurt::instance());
		return true;
	}
	return false;
}