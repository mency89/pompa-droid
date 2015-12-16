#ifndef __MESSAGETYPES_H__
#define __MESSAGETYPES_H__

#include "base/CCEventKeyboard.h"

enum MessageTypes
{
	msg_KeyPressed = 1000,
	msg_KeyReleased = 1001,
	msg_EntityMove = 2001,
	msg_EntityHurt = 2002,
	msg_EntityAttack = 2003
};

struct MSKeyPressed
{
	cocos2d::EventKeyboard::KeyCode key_code;
};

struct MSKeyReleased
{
	cocos2d::EventKeyboard::KeyCode key_code;
};

#endif