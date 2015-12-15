#include "HeroOwnedStates.h"

#include "ActionTags.h"
#include "MeesageTypes.h"
#include "AnimationManger.h"
#include "MessageDispatcher.h"
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
			keyCode == EventKeyboard::KeyCode::KEY_D;
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
			if (IsJumpKey(keyCode))
			{
				object->getStateMachine()->change_state(HeroJump::instance());
				return true;
			}
			else if (IsAttackKey(keyCode))
			{
				object->getStateMachine()->change_state(HeroAttack::instance());
				return true;
			}
			else if (IsDirectionKey(keyCode))
			{
				BaseGameEntity::Direction direction = ConvertDirectionKeyToHeroDirection(keyCode);
				if (direction == object->getDirection() && direction != BaseGameEntity::Up && direction != BaseGameEntity::Down)
				{
					std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point direction_key_pressed_time = object->getStateMachine()->userdata().direction_key_pressed_time;
					std::chrono::system_clock::duration duration = current_time - direction_key_pressed_time;
					if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() < 250)
					{
						object->getStateMachine()->change_state(HeroRun::instance());
					}
					else
					{
						object->getStateMachine()->change_state(HeroWalk::instance());
					}
				}
				else
				{
					object->setDirection(direction);
					object->getStateMachine()->change_state(HeroWalk::instance());
				}
				object->getStateMachine()->userdata().direction_key_pressed_time = std::chrono::system_clock::now();
				return true;
			}
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
}

bool HeroWalk::on_message(Hero *object, const Telegram &msg)
{
	if (MessageTypes::msg_KeyPressed == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsJumpKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroJump::instance());
			return true;
		}
		else if (IsAttackKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroAttack::instance());
			return true;
		}
	}

	if (MessageTypes::msg_KeyReleased == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsDirectionKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroIdle::instance());
			return true;
		}
	}
	return false;
}

/******英雄奔跑状态******/

void HeroRun::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_run");
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.08f);
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
	object->move(object->getRunSpeed());
}

bool HeroRun::on_message(Hero *object, const Telegram &msg)
{
	if (MessageTypes::msg_KeyPressed == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsJumpKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroJump::instance());
			return true;
		}
		else if (IsAttackKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroRuningAttack::instance());
			return true;
		}
	}

	if (MessageTypes::msg_KeyReleased == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsDirectionKey(keyCode))
		{
			object->getStateMachine()->change_state(HeroIdle::instance());
			return true;
		}
	}
	return false;
}

/******英雄跳跃状态******/

void HeroJump::enter(Hero *object)
{
	object->getStateMachine()->userdata().jump_up = true;
	object->getStateMachine()->userdata().before_he_height = object->getPositionY();
	Animation *animation = AnimationManger::instance()->getAnimation("hero_jump");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_jump);
	object->runAction(animate);
}

void HeroJump::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_jump);
}

void HeroJump::execute(Hero *object)
{
	if (object->getStateMachine()->userdata().jump_up)
	{
		if (object->getPositionY() < object->getStateMachine()->userdata().before_he_height + object->getMaxJumpHeight())
		{
			object->setPositionY(object->getPositionY() + object->getJumpForce());
		}
		else
		{
			object->getStateMachine()->userdata().jump_up = false;
			object->setPositionY(object->getPositionY() - object->getJumpForce());
		}
	}
	else
	{
		object->setPositionY(object->getPositionY() - object->getJumpForce());
	}

	auto previous_state = object->getStateMachine()->get_previous_state();
	if (previous_state != nullptr)
	{
		if (previous_state == HeroWalk::instance())
		{
			object->move(object->getWalkSpeed());
		}
		else if (previous_state == HeroRun::instance())
		{
			object->move(object->getRunSpeed());
		}
	}

	if (!object->getStateMachine()->userdata().jump_up &&
		object->getPositionY() < object->getStateMachine()->userdata().before_he_height)
	{
		object->setPositionY(object->getStateMachine()->userdata().before_he_height);
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroJump::on_message(Hero *object, const Telegram &msg)
{
	if (MessageTypes::msg_KeyPressed == msg.msg_code)
	{
		EventKeyboard::KeyCode keyCode = *reinterpret_cast<const EventKeyboard::KeyCode *>(msg.extra_info);
		if (IsAttackKey(keyCode))
		{
			if (object->getPositionY() >= object->getStateMachine()->userdata().before_he_height + object->getMaxJumpHeight() / 2)
			{
				object->getStateMachine()->change_state(HeroJumpingAttack::instance());
			}		
			return true;
		}
	}
	return false;
}

/******英雄攻击状态******/

void HeroAttack::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_attack_00");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_attack);
	object->runAction(animate);
}

void HeroAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_attack);
}

void HeroAttack::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::hero_attack) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroAttack::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄奔跑攻击状态******/

void HeroRuningAttack::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_runattack");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_runattack);
	object->runAction(animate);
}

void HeroRuningAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_runattack);
}

void HeroRuningAttack::execute(Hero *object)
{
	object->move(object->getRunSpeed());
	if (object->getActionByTag(ActionTags::hero_runattack) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroRuningAttack::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄跳跃攻击状态******/

void HeroJumpingAttack::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_jumpattack");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_jumpattack);
	object->runAction(animate);
}

void HeroJumpingAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_jumpattack);
}

void HeroJumpingAttack::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::hero_jumpattack) == nullptr)
	{
		object->setPositionY(object->getPositionY() - object->getJumpForce());

		if (object->getPositionY() < object->getStateMachine()->userdata().before_he_height)
		{
			object->setPositionY(object->getStateMachine()->userdata().before_he_height);
			object->getStateMachine()->change_state(HeroIdle::instance());
		}
	}
}

bool HeroJumpingAttack::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄受击状态******/

void HeroHurt::enter(Hero *object)
{
	if (++object->getStateMachine()->userdata().be_continuous_attack < 3)
	{
		object->stopActionByTag(ActionTags::hero_hurt);
		Animation *animation = AnimationManger::instance()->getAnimation("hero_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::hero_hurt);
		object->runAction(animate);
	}
}

void HeroHurt::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_hurt);
}

void HeroHurt::execute(Hero *object)
{
	if (object->getStateMachine()->userdata().be_continuous_attack >= 3)
	{
		object->getStateMachine()->change_state(HeroKnockout::instance());
		object->getStateMachine()->userdata().be_continuous_attack = 0;
	}
	else if (object->getActionByTag(ActionTags::hero_hurt) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
		object->getStateMachine()->userdata().be_continuous_attack = 0;
	}
}

bool HeroHurt::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄倒下状态******/

void HeroKnockout::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_knockout");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_knockout);
	object->runAction(animate);
}

void HeroKnockout::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_knockout);
}

void HeroKnockout::execute(Hero *object)
{
	if (object->isFlippedX())
	{
		object->setPositionX(object->getPositionX() + object->getRunSpeed());
	}
	else
	{
		object->setPositionX(object->getPositionX() - object->getRunSpeed());
	}

	if (object->getActionByTag(ActionTags::hero_knockout) == nullptr)
	{
		object->getStateMachine()->change_state(HeroGetup::instance());
	}
}

bool HeroKnockout::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄起身状态******/

void HeroGetup::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_getup");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::hero_getup);
	object->runAction(animate);
}

void HeroGetup::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::hero_getup);
}

void HeroGetup::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::hero_getup) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroGetup::on_message(Hero *object, const Telegram &msg)
{
	return false;
}

/******英雄全局状态******/

void HeroGlobal::enter(Hero *object)
{
}

void HeroGlobal::exit(Hero *object)
{
}

void HeroGlobal::execute(Hero *object)
{
	std::vector<BaseGameEntity*> targets = object->getHitTargets();
	for (BaseGameEntity *entity : targets)
	{
		Telegram msg;
		msg.sender = object->get_id();
		msg.receiver = entity->get_id();
		msg.msg_code = msg_Hurt;
		MessageDispatcher::instance()->dispatchMessage(msg);
	}
}

bool HeroGlobal::on_message(Hero *object, const Telegram &msg)
{
	return false;
}