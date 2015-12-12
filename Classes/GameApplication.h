#ifndef __GAMEAPPLICATION_H__
#define __GAMEAPPLICATION_H__

#include "cocos2d.h"
#include "Singleton.h"

class GameApplication : public Singleton < GameApplication >, public cocos2d::Node
{
	SINGLETON(GameApplication);

private:
	virtual bool init() override;

	virtual void update(float delta) override;
};

#endif