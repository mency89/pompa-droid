#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

class Hero;

class GameScene : public cocos2d::Layer
{
public:
	GameScene();

	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init() override;

	virtual void update(float delta) override;

	CREATE_FUNC(GameScene);

private:
	Hero* hero_;
};

#endif