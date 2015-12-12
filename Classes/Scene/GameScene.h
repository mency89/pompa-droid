#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <memory>
#include "cocos2d.h"

class Hero;
class b2World;

class GameScene : public cocos2d::Layer
{
public:
	GameScene();

	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init() override;

	virtual void update(float delta) override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	CREATE_FUNC(GameScene);

private:
	Hero* hero_;
	std::shared_ptr<b2World> world_;
};

#endif