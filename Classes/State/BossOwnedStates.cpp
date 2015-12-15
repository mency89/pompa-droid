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
	animate->setTag(ActionTags::boos_idle);
	object->runAction(animate);
}

void BossIdle::exit(Boss *object)
{
	object->stopActionByTag(ActionTags::boos_idle);
}

void BossIdle::execute(Boss *object)
{
}

bool BossIdle::on_message(Boss *object, const Telegram &msg)
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
		return true;
	}
	return false;
}