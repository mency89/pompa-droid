#ifndef __MESSAGETYPES_H__
#define __MESSAGETYPES_H__

#include "math/Vec2.h"
#include "base/CCEventKeyboard.h"

enum MessageTypes
{
	kMsgKeyPressed = 1001,
	kMsgKeyReleased = 1002,
	kMsgEntityMove = 2001,
	kMsgEntityHurt = 2002,
	kMsgEntityAttack = 2003
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
	unsigned int value;
};

#endif