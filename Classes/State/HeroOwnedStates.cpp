#include "HeroOwnedStates.h"

#include "ActionTags.h"
#include "MeesageTypes.h"
#include "AnimationManger.h"
#include "GameApplication.h"
using namespace cocos2d;

namespace
{
	bool IsOpenKeyCode(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_W ||
			keyCode == EventKeyboard::KeyCode::KEY_S ||
			keyCode == EventKeyboard::KeyCode::KEY_A ||
			keyCode == EventKeyboard::KeyCode::KEY_D ||
			keyCode == EventKeyboard::KeyCode::KEY_J ||
			keyCode == EventKeyboard::KeyCode::KEY_SPACE;
	}

	bool IsDirectionKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_W ||
			keyCode == EventKeyboard::KeyCode::KEY_S ||
			keyCode == EventKeyboard::KeyCode::KEY_A ||
			keyCode == EventKeyboard::KeyCode::KEY_D ||
			keyCode == EventKeyboard::KeyCode::KEY_J;
	}

	bool IsAttackKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_J;
	}

	bool IsJumpKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_SPACE;
	}

	BaseGameEntity::Direction ConvertDirectionKeyToHeroDirection(EventKeyboard::KeyCode keyCode)
	{
		CCAssert(IsDirectionKey(keyCode), "");
		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			return BaseGameEntity::Direction::Up;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			return BaseGameEntity::Direction::Down;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			return BaseGameEntity::Direction::Left;
		}
		else
		{
			return BaseGameEntity::Direction::Right;
		}
	}
}


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

}

bool HeroIdle::on_message(Hero *object, const Telegram &msg)
{
	if (MessageTypes::msg_KeyPressed == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsOpenKeyCode(keyCode))
		{
			if (IsDirectionKey(keyCode))
			{
				object->setDirection(ConvertDirectionKeyToHeroDirection(keyCode));
				object->getStateMachine()->change_state(HeroWalk::instance());
			}
			return true;
		}
	}
	return false;
}

/******英雄行走状态******/

void HeroWalk::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_walk");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_walk);
	object->runAction(animate);
}

void HeroWalk::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_walk);
}

void HeroWalk::execute(Hero *object)
{
	object->move(object->getWalkSpeed());
	if (object->getActionByTag(ActionTags::hero_walk) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroWalk::on_message(Hero *object, const Telegram &msg)
{
	if (MessageTypes::msg_KeyReleased == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsOpenKeyCode(keyCode))
		{
			if (IsDirectionKey(keyCode))
			{
				object->setDirection(ConvertDirectionKeyToHeroDirection(keyCode));
				object->getStateMachine()->change_state(HeroIdle::instance());
			}
			return true;
		}
	}
	return false;
}