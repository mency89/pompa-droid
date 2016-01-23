#ifndef __SIMPLEROBOTLOGIC_H__
#define __SIMPLEROBOTLOGIC_H__


template <typename EntityType, typename IdleState, typename IdleLittleWhileState, typename MoveState, typename AttackState>
void SimpleRobotLogic(EntityType *object)
{
	// 决策系统
	LevelLayer *current_level = object->getEntityManger()->getCurrentLevel();
	if (IdleState::instance() == object->getStateMachine()->get_current_state())
	{
		// 面向玩家
		Hero *hero = current_level->getHero();
		if (hero->getPositionX() < object->getPositionX())
		{
			object->setDirection(BaseGameEntity::kLeftDirection);
		}
		else if (hero->getPositionX() > object->getPositionX())
		{
			object->setDirection(BaseGameEntity::kRightDirection);
		}

		if (!hero->isDeath())
		{
			if (current_level->isAdjacent(object, hero))
			{
				// 如果玩家在攻击范围内
				if (rand() % 2 == 0)
				{
					// 休息一会儿
					object->getStateMachine()->userdata().end_resting_time = std::chrono::system_clock::now()
						+ std::chrono::milliseconds(rand() % 2000);
					object->getStateMachine()->change_state(IdleLittleWhileState::instance());
				}
				else
				{
					// 攻击玩家
					object->getStateMachine()->change_state(AttackState::instance());
				}
			}
			else
			{
				if (rand() % 2 == 0)
				{
					// 休息一会儿
					object->getStateMachine()->userdata().end_resting_time = std::chrono::system_clock::now()
						+ std::chrono::milliseconds(rand() % 1000);
					object->getStateMachine()->change_state(IdleLittleWhileState::instance());
				}
				else
				{
					// 靠近玩家
					if (!hero->isJumpingState() && current_level->insideOfFloor(hero))
					{
						Vec2 temp = object->getPosition();
						Vec2 hero_pos = current_level->getRealEntityPosition(hero);
						if (hero->getPositionX() < object->getPositionX())
						{
							hero_pos.x = hero_pos.x + hero->realWidth() / 2 + object->realWidth() / 2;
						}
						else
						{
							hero_pos.x = hero_pos.x - hero->realWidth() / 2 - object->realWidth() / 2;
						}
						Vec2 &target_pos = object->getStateMachine()->userdata().target_pos;
						current_level->setRealEntityPosition(object, hero_pos);
						target_pos = object->getPosition();
						int offset = current_level->getTileSize().height / 2;
						target_pos.y += rand() % 2 == 0 ? -(rand() % offset) : rand() % offset;
						object->setPosition(temp);
						object->getStateMachine()->change_state(MoveState::instance());
					}
				}
			}
		}
	}

	// 判断是否攻击到目标
	if (object->getStateMachine()->get_current_state() == AttackState::instance())
	{
		auto targets = object->getHitTargets();
		for (auto &collision : targets)
		{
			if (!object->getStateMachine()->userdata().hit_hero)
			{
				Message msg;
				msg.sender = object->getID();
				msg.receiver = collision.entity->getID();
				msg.msg_code = kMsgEntityHurt;

				STEntityHurt extra_info;
				extra_info.pos = collision.pos;
				extra_info.value = object->getAttack();
				msg.extra_info = &extra_info;
				msg.extra_info_size = sizeof(STEntityHurt);

				MessageDispatcher::instance()->dispatchMessage(msg);
				object->getStateMachine()->userdata().hit_hero = true;
			}
		}
	}
}

#endif