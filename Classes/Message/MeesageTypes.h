#ifndef __MESSAGETYPES_H__
#define __MESSAGETYPES_H__

#include "math/Vec2.h"
#include "base/CCEventKeyboard.h"

enum MessageTypes
{
	msg_KeyPressed = 1000,
	msg_KeyReleased = 1001,
	msg_EntityMove = 2001,
	msg_EntityHurt = 2002,
	msg_EntityAttack = 2003
};

struct STKeyPressed
{
	cocos2d::EventKeyboard::KeyCode key_code;
};

struct STKeyReleased
{
	cocos2d::EventKeyboard::KeyCode key_code;
};

struct STEntityHurt
{
	cocos2d::Vec2 pos;
};

#endif