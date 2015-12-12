#ifndef __GAMEAPPLICATION_H__
#define __GAMEAPPLICATION_H__

#include "cocos2d.h"
#include "Singleton.h"

class GameApplication : public Singleton < GameApplication >, public cocos2d::Node
{
	SINGLETON(GameApplication);

public:
	cocos2d::EventKeyboard::KeyCode takeKeyPressed();

	cocos2d::EventKeyboard::KeyCode takeKeyReleased();

	void setKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);

	void setKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);

private:
	virtual bool init() override;

	virtual void update(float delta) override;

private:
	cocos2d::EventKeyboard::KeyCode pressed_;
	cocos2d::EventKeyboard::KeyCode released_;
};

#endif