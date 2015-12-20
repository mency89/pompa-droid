#ifndef __GAMEAPPLICATION_H__
#define __GAMEAPPLICATION_H__

#include "cocos2d.h"
#include "Singleton.h"

class GameScene;

class GameApplication : public Singleton < GameApplication >, public cocos2d::Node
{
	SINGLETON(GameApplication);

public:
	GameScene* getGameScene();

	void setGameScene(GameScene *scene);

private:
	virtual bool init() override;

	virtual void update(float delta) override;

private:
	GameScene* scene_;
};

#endif