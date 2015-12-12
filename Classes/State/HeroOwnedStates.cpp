#include "HeroOwnedStates.h"

#include "ActionTags.h"
#include "AnimationManger.h"
#include "GameApplication.h"
using namespace cocos2d;


/******英雄待机状态******/

void HeroIdle::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_idle");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_idle);
	object->runAction(animate);
}

void HeroIdle::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_idle);
}

void HeroIdle::execute(Hero *object)
{
	/*cocos2d::EventKeyboard::KeyCode key = GameApplication::instance()->takeKeyPressed();
	if (key != EventKeyboard::KeyCode::KEY_NONE)
	{
		switch (key)
		{
			case EventKeyboard::KeyCode::KEY_W:
			{
				object->setDirection(BaseGameEntity::Up);
				object->getStateMachine()->change_state(HeroRun::instance());
				break;
			}
			case EventKeyboard::KeyCode::KEY_S:
			{
				object->setDirection(BaseGameEntity::Down);
				object->getStateMachine()->change_state(HeroRun::instance());
				break;
			}
			case EventKeyboard::KeyCode::KEY_A:
			{
				object->setDirection(BaseGameEntity::Left);
				object->getStateMachine()->change_state(HeroRun::instance());
				break;
			}
			case EventKeyboard::KeyCode::KEY_D:
			{
				object->setDirection(BaseGameEntity::Right);
				object->getStateMachine()->change_state(HeroRun::instance());
				break;
			}
		}
	}*/
}

bool HeroIdle::on_message(Hero *object, const Telegram &msg)
{
	return true;
}

/******英雄行走状态******/

void HeroRun::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_run");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_run);
	object->runAction(animate);
}

void HeroRun::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_run);
}

void HeroRun::execute(Hero *object)
{
	/*bool stop = true;
	cocos2d::EventKeyboard::KeyCode key = GameApplication::instance()->takeKeyPressed();
	if (key != EventKeyboard::KeyCode::KEY_NONE)
	{
		switch (key)
		{
			case EventKeyboard::KeyCode::KEY_W:
			{
				stop = false;
				object->setDirection(BaseGameEntity::Up);
				break;
			}
			case EventKeyboard::KeyCode::KEY_S:
			{
				stop = false;
				object->setDirection(BaseGameEntity::Down);
				break;
			}
			case EventKeyboard::KeyCode::KEY_A:
			{
				stop = false;
				object->setDirection(BaseGameEntity::Left);
				break;
			}
			case EventKeyboard::KeyCode::KEY_D:
			{
				stop = false;
				object->setDirection(BaseGameEntity::Right);
				break;
			}
		}
	}

	if (stop)
	{
		cocos2d::EventKeyboard::KeyCode key = GameApplication::instance()->takeKeyReleased();
		if (key != EventKeyboard::KeyCode::KEY_NONE)
		{
			object->getStateMachine()->change_state(HeroIdle::instance());
		}
	}*/
}

bool HeroRun::on_message(Hero *object, const Telegram &msg)
{
	return false;
}