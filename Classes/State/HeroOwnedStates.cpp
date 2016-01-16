#include "HeroOwnedStates.h"

#include "ActionTags.h"
#include "Entity/Boss.h"
#include "Entity/Weapon.h"
#include "AnimationManger.h"
#include "Scene/LevelLayer.h"
#include "Entity/EntityManger.h"
#include "Message/MeesageTypes.h"
#include "Message/MessageDispatcher.h"
using namespace cocos2d;
using namespace std::chrono;

namespace
{
	// 是否有效的按键
	bool IsValidKeyCode(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_W ||
			keyCode == EventKeyboard::KeyCode::KEY_S ||
			keyCode == EventKeyboard::KeyCode::KEY_A ||
			keyCode == EventKeyboard::KeyCode::KEY_D ||
			keyCode == EventKeyboard::KeyCode::KEY_J ||
			keyCode == EventKeyboard::KeyCode::KEY_SPACE;
	}

	// 是否是方向键
	bool IsDirectionKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_W ||
			keyCode == EventKeyboard::KeyCode::KEY_S ||
			keyCode == EventKeyboard::KeyCode::KEY_A ||
			keyCode == EventKeyboard::KeyCode::KEY_D;
	}

	// 是否是攻击键
	bool IsAttackKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_J;
	}

	// 是否是跳跃键
	bool IsJumpKey(EventKeyboard::KeyCode keyCode)
	{
		return keyCode == EventKeyboard::KeyCode::KEY_SPACE;
	}

	// 转换方向键为英雄方向
	BaseGameEntity::Direction ConvertDirectionKeyToHeroDirection(EventKeyboard::KeyCode keyCode)
	{
		CCAssert(IsDirectionKey(keyCode), "");
		if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			return BaseGameEntity::kUpDirection;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_S)
		{
			return BaseGameEntity::kDownDirection;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			return BaseGameEntity::kLeftDirection;
		}
		else
		{
			return BaseGameEntity::kRightDirection;
		}
	}

	// 是否是攻击状态
	bool IsAttackState(State<Hero> *state)
	{
		return state == HeroAttack::instance() || state == HeroRuningAttack::instance() || state == HeroJumpingAttack::instance();
	}
}


/******英雄待机状态******/

void HeroIdle::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_idle");
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kHeroIdle);
	object->runAction(animate);

	if (object->hasWeapon())
	{
		animation = AnimationManger::instance()->getAnimation("weapon_idle");
		animation->setLoops(-1);
		animate = Animate::create(animation);
		animate->setTag(ActionTags::kWeaponIdle);
		object->getWeaponSkin()->runAction(animate);
		object->getWeaponSkin()->setVisible(true);
	}
}

void HeroIdle::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroIdle);
	if (object->hasWeapon())
	{
		object->getWeaponSkin()->setVisible(false);
		object->getWeaponSkin()->stopActionByTag(ActionTags::kWeaponIdle);
	}
}

void HeroIdle::execute(Hero *object)
{

}

bool HeroIdle::on_message(Hero *object, const Message &msg)
{
	if (MessageTypes::kMsgKeyPressed == msg.msg_code)
	{
		STKeyPressed extra_info = *reinterpret_cast<const STKeyPressed*>(msg.extra_info);
		if (IsValidKeyCode(extra_info.key_code))
		{
			if (IsJumpKey(extra_info.key_code))
			{
				object->getStateMachine()->change_state(HeroJump::instance());
				return true;
			}
			else if (IsAttackKey(extra_info.key_code))
			{
				object->getStateMachine()->change_state(HeroAttack::instance());
				return true;
			}
			else if (IsDirectionKey(extra_info.key_code))
			{
				BaseGameEntity::Direction direction = ConvertDirectionKeyToHeroDirection(extra_info.key_code);
				if (direction == object->getDirection() && direction != BaseGameEntity::kUpDirection && direction != BaseGameEntity::kDownDirection)
				{
					system_clock::time_point current_time = system_clock::now();
					system_clock::time_point last_direction_key_pressed_time = object->getStateMachine()->userdata().last_direction_key_pressed_time;
					system_clock::duration duration = current_time - last_direction_key_pressed_time;
					if (duration_cast<milliseconds>(duration).count() < 250)
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
				object->getStateMachine()->userdata().last_direction_key_pressed_time = system_clock::now();
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
	animate->setTag(ActionTags::kHeroWalk);
	object->runAction(animate);

	if (object->hasWeapon())
	{
		animation = AnimationManger::instance()->getAnimation("weapon_walk");
		animation->setLoops(-1);
		animate = Animate::create(animation);
		animate->setTag(ActionTags::kWeaponWalk);
		object->getWeaponSkin()->runAction(animate);
		object->getWeaponSkin()->setVisible(true);
	}
}

void HeroWalk::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroWalk);

	if (object->hasWeapon())
	{
		object->getWeaponSkin()->setVisible(false);
		object->getWeaponSkin()->stopActionByTag(ActionTags::kWeaponWalk);
	}
}

void HeroWalk::execute(Hero *object)
{
	object->move(object->getWalkSpeed());
	object->getEntityManger()->getCurrentLevel()->adjustmentPosition(object);
}

bool HeroWalk::on_message(Hero *object, const Message &msg)
{
	if (MessageTypes::kMsgKeyPressed == msg.msg_code)
	{
		STKeyPressed extra_info = *reinterpret_cast<const STKeyPressed *>(msg.extra_info);
		if (IsJumpKey(extra_info.key_code))
		{
			object->getStateMachine()->change_state(HeroJump::instance());
			return true;
		}
		else if (IsAttackKey(extra_info.key_code))
		{
			object->getStateMachine()->change_state(HeroAttack::instance());
			return true;
		}
	}
	else if (MessageTypes::kMsgKeyReleased == msg.msg_code)
	{
		STKeyReleased extra_info = *reinterpret_cast<const STKeyReleased *>(msg.extra_info);
		if (IsDirectionKey(extra_info.key_code))
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
	animate->setTag(ActionTags::kHeroRun);
	object->runAction(animate);

	if (object->hasWeapon())
	{
		auto current_level = object->getEntityManger()->getCurrentLevel();
		current_level->dropWeapon(object);
	}
}

void HeroRun::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroRun);
}

void HeroRun::execute(Hero *object)
{
	object->move(object->getRunSpeed());
	object->getEntityManger()->getCurrentLevel()->adjustmentPosition(object);
}

bool HeroRun::on_message(Hero *object, const Message &msg)
{
	if (MessageTypes::kMsgKeyPressed == msg.msg_code)
	{
		STKeyPressed extra_info = *reinterpret_cast<const STKeyPressed *>(msg.extra_info);
		if (IsJumpKey(extra_info.key_code))
		{
			object->getStateMachine()->change_state(HeroJump::instance());
			return true;
		}
		else if (IsAttackKey(extra_info.key_code))
		{
			object->getStateMachine()->change_state(HeroRuningAttack::instance());
			return true;
		}
	}
	else if (MessageTypes::kMsgKeyReleased == msg.msg_code)
	{
		STKeyReleased extra_info = *reinterpret_cast<const STKeyReleased *>(msg.extra_info);
		if (IsDirectionKey(extra_info.key_code))
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
	object->getStateMachine()->userdata().before_jump_y = object->getPositionY();
	Animation *animation = AnimationManger::instance()->getAnimation("hero_jump");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kHeroJump);
	object->runAction(animate);

	if (object->hasWeapon())
	{
		auto current_level = object->getEntityManger()->getCurrentLevel();
		current_level->dropWeapon(object);
	}
}

void HeroJump::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroJump);
}

void HeroJump::execute(Hero *object)
{
	if (object->getStateMachine()->userdata().jump_up)
	{
		if (object->getPositionY() < object->getStateMachine()->userdata().before_jump_y + object->getMaxJumpHeight())
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
		object->getEntityManger()->getCurrentLevel()->adjustmentPositionX(object);
	}

	if (!object->getStateMachine()->userdata().jump_up &&
		object->getPositionY() < object->getStateMachine()->userdata().before_jump_y)
	{
		object->setPositionY(object->getStateMachine()->userdata().before_jump_y);
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroJump::on_message(Hero *object, const Message &msg)
{
	if (MessageTypes::kMsgKeyPressed == msg.msg_code)
	{
		STKeyPressed extra_info = *reinterpret_cast<const STKeyPressed *>(msg.extra_info);
		if (IsAttackKey(extra_info.key_code))
		{
			if (object->getPositionY() >= object->getStateMachine()->userdata().before_jump_y + object->getMaxJumpHeight() / 2)
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
	if (object->getEntityManger()->getCurrentLevel()->pickUpWeapon(object))
	{
		object->getStateMachine()->userdata().hit_target_count = 0;
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
	else
	{
		system_clock::time_point current_time = system_clock::now();
		system_clock::time_point last_hurt_time = object->getStateMachine()->userdata().hit_target_time;
		system_clock::duration duration = current_time - last_hurt_time;
		if (duration_cast<milliseconds>(duration).count() < 1000)
		{
			++object->getStateMachine()->userdata().hit_target_count;
		}
		else
		{
			object->getStateMachine()->userdata().hit_target_count = 1;
		}

		char str[32];
		sprintf(str, "hero_attack_0%d", object->getStateMachine()->userdata().hit_target_count - 1);
		Animation *animation = AnimationManger::instance()->getAnimation(str);
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::kHeroAttcak);
		object->runAction(animate);

		if (object->hasWeapon())
		{
			sprintf(str, "weapon_attack_0%d", object->getStateMachine()->userdata().hit_target_count - 1);
			animation = AnimationManger::instance()->getAnimation(str);
			animate = Animate::create(animation);
			animate->setTag(ActionTags::kWeaponAttack);
			object->getWeaponSkin()->runAction(animate);
			object->getWeaponSkin()->setVisible(true);
		}
	}
}

void HeroAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroAttcak);
	if (object->hasWeapon())
	{
		object->getWeaponSkin()->setVisible(false);
		object->getWeaponSkin()->stopActionByTag(ActionTags::kWeaponAttack);
	}
	if (object->getStateMachine()->userdata().hit_target_count >= 3)
	{
		object->getStateMachine()->userdata().hit_target_count = 0;
	}
}

void HeroAttack::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::kHeroAttcak) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroAttack::on_message(Hero *object, const Message &msg)
{
	return false;
}

/******英雄奔跑攻击状态******/

void HeroRuningAttack::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_runattack");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kHeroRunAttcak);
	object->runAction(animate);
}

void HeroRuningAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroRunAttcak);
}

void HeroRuningAttack::execute(Hero *object)
{
	object->move(object->getWalkSpeed());
	object->getEntityManger()->getCurrentLevel()->adjustmentPosition(object);
	if (object->getActionByTag(ActionTags::kHeroRunAttcak) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroRuningAttack::on_message(Hero *object, const Message &msg)
{
	return false;
}

/******英雄跳跃攻击状态******/

void HeroJumpingAttack::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_jumpattack");
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kHeroJumpAttcak);
	object->runAction(animate);
}

void HeroJumpingAttack::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroJumpAttcak);
}

void HeroJumpingAttack::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::kHeroJumpAttcak) == nullptr)
	{
		object->setPositionY(object->getPositionY() - object->getJumpForce());
		if (object->getPositionY() < object->getStateMachine()->userdata().before_jump_y)
		{
			object->setPositionY(object->getStateMachine()->userdata().before_jump_y);
			object->getStateMachine()->change_state(HeroIdle::instance());
		}
	}
}

bool HeroJumpingAttack::on_message(Hero *object, const Message &msg)
{
	return false;
}

/******英雄受击状态******/

void HeroHurt::enter(Hero *object)
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

	if (object->getStateMachine()->userdata().was_hit_count < 3)
	{
		Animation *animation = AnimationManger::instance()->getAnimation("hero_hurt");
		Animate *animate = Animate::create(animation);
		animate->setTag(ActionTags::kHeroHurt);
		object->runAction(animate);
		object->getStateMachine()->userdata().was_hit_time = system_clock::now();
	}
	else
	{
		object->getStateMachine()->change_state(HeroKnockout::instance());
	}

	if (object->hasWeapon())
	{
		auto current_level = object->getEntityManger()->getCurrentLevel();
		current_level->dropWeapon(object);
	}
}

void HeroHurt::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroHurt);
}

void HeroHurt::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::kHeroHurt) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroHurt::on_message(Hero *object, const Message &msg)
{
	return false;
}

/******英雄倒下状态******/

void HeroKnockout::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_knockout");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kHeroKnockout);
	object->runAction(animate);
}

void HeroKnockout::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kHeroKnockout);
}

void HeroKnockout::execute(Hero *object)
{
	object->stepback(1.0f);
	object->getEntityManger()->getCurrentLevel()->adjustmentPositionX(object);

	if (HeroJump::instance() == object->getStateMachine()->get_previous_state() ||
		HeroJumpingAttack::instance() == object->getStateMachine()->get_previous_state())
	{
		object->setPositionY(object->getPositionY() - object->getJumpForce());
		if (object->getPositionY() < object->getStateMachine()->userdata().before_jump_y)
		{
			object->setPositionY(object->getStateMachine()->userdata().before_jump_y);
		}
	}

	if (object->getActionByTag(ActionTags::kHeroKnockout) == nullptr)
	{
		if (object->isDeath())
		{
			object->getStateMachine()->set_current_state(nullptr);
			object->getStateMachine()->set_global_state(nullptr);
		}
		else
		{
			object->getStateMachine()->change_state(HeroGetup::instance());
		}
	}
}

bool HeroKnockout::on_message(Hero *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
}

/******英雄起身状态******/

void HeroGetup::enter(Hero *object)
{
	Animation *animation = AnimationManger::instance()->getAnimation("hero_getup");
	animation->setRestoreOriginalFrame(false);
	Animate *animate = Animate::create(animation);
	animate->setTag(ActionTags::kheroGetup);
	object->runAction(animate);
}

void HeroGetup::exit(Hero *object)
{
	object->stopActionByTag(ActionTags::kheroGetup);
}

void HeroGetup::execute(Hero *object)
{
	if (object->getActionByTag(ActionTags::kheroGetup) == nullptr)
	{
		object->getStateMachine()->change_state(HeroIdle::instance());
	}
}

bool HeroGetup::on_message(Hero *object, const Message &msg)
{
	// 吞噬受击消息
	return msg.msg_code == kMsgEntityHurt;
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
	// 判断是否攻击到目标
	LevelLayer *current_level = object->getEntityManger()->getCurrentLevel();
	if (IsAttackState(object->getStateMachine()->get_current_state()))
	{
		auto targets = object->getHitTargets();
		for (auto &collision : targets)
		{
			auto result = object->getStateMachine()->userdata().hit_targets.find(collision.entity->getID());
			if (result == object->getStateMachine()->userdata().hit_targets.end())
			{
				bool adjacent = current_level->isAdjacent(object, collision.entity);
				if (HeroJumpingAttack::instance() == object->getStateMachine()->get_current_state())
				{
					float y = object->getPositionY();
					object->setPositionY(object->getStateMachine()->userdata().before_jump_y);
					Vec2 a = current_level->getRealEntityPosition(object);
					Vec2 b = current_level->getRealEntityPosition(collision.entity);
					adjacent = abs(b.y - a.y) < current_level->getMapSize().height;				
					object->setPositionY(y);
				}

				if (adjacent)
				{
					Message msg;
					msg.sender = object->getID();
					msg.receiver = collision.entity->getID();
					msg.msg_code = kMsgEntityHurt;

					STEntityHurt extra_info;
					extra_info.local_pos = collision.collision_pos;
					msg.extra_info = &extra_info;
					msg.extra_info_size = sizeof(STEntityHurt);

					if (object->getStateMachine()->get_current_state() == HeroAttack::instance())
					{
						extra_info.value = object->getStateMachine()->userdata().hit_target_count >= 3 ? object->getRunAttack() : object->getAttack();
					}
					else if (object->getStateMachine()->get_current_state() == HeroRuningAttack::instance())
					{
						extra_info.value = object->getRunAttack();
					}
					else if (object->getStateMachine()->get_current_state() == HeroJumpingAttack::instance())
					{
						extra_info.value = object->getJumpAttack();
					}

					MessageDispatcher::instance()->dispatchMessage(msg);
					if (object->getStateMachine()->get_current_state() == HeroAttack::instance())
					{
						object->getStateMachine()->userdata().hit_target_time = system_clock::now();
					}
					object->getStateMachine()->userdata().hit_targets.insert(collision.entity->getID());
				}
			}	
		}
	}
	else
	{
		object->getStateMachine()->userdata().hit_targets.clear();
	}
}

bool HeroGlobal::on_message(Hero *object, const Message &msg)
{
	if (msg.msg_code == kMsgEntityHurt)
	{
		STEntityHurt extra_info = *reinterpret_cast<const STEntityHurt*>(msg.extra_info);
		object->getStateMachine()->userdata().hurt_source = msg.sender;
		object->getEntityManger()->getCurrentLevel()->playHitEffect(extra_info.local_pos, extra_info.value);
		object->hurtLife(extra_info.value);
		if (object->isDeath())
		{
			object->getStateMachine()->change_state(HeroKnockout::instance());
		}
		else
		{
			if (HeroJump::instance() == object->getStateMachine()->get_current_state() ||
				HeroJumpingAttack::instance() == object->getStateMachine()->get_current_state())
			{
				object->getStateMachine()->change_state(HeroKnockout::instance());
			}
			else if (dynamic_cast<Boss *>(object->getEntityManger()->getEntityByID(msg.sender)) != nullptr)
			{
				object->getStateMachine()->change_state(HeroKnockout::instance());
			}
			else
			{
				object->getStateMachine()->change_state(HeroHurt::instance());
			}
		}
		return true;
	}
	return false;
}