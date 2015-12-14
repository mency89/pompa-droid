#include "BossOwnedStates.h"

#include "ActionTags.h"
#include "MeesageTypes.h"
#include "AnimationManger.h"
using namespace cocos2d;


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
	return false;
}