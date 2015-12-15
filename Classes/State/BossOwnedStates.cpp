#include "BossOwnedStates.h"

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
	if (++object->getStateMachine()->userdata().continuous_hurt < 3)
	{
		Animation *animation = AnimationManger::instance()->getAnimation("boss_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::boss_hurt);
		object->runAction(animate);
	}
}

void BossHurt::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boss_hurt);
}

void BossHurt::execute(Boss *object)
{
	if (object->getStateMachine()->userdata().continuous_hurt >= 3)
	{
		object->getStateMachine();
		/*	object->getStateMachine()->change_state(HeroKnockout::instance());
			object->getStateMachine()->userdata().continuous_hurt = 0;*/
	}
	else if (object->getActionByTag(ActionTags::boss_hurt) == nullptr)
	{
		object->getStateMachine()->change_state(BossIdle::instance());
		object->getStateMachine()->userdata().continuous_hurt = 0;
	}
}

bool BossHurt::on_message(Boss *object, const Telegram &msg)
{
	return false;
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