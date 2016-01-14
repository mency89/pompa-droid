#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <memory>
#include "cocos2d.h"

class b2World;
class LevelLayer;
class GLESDebugDraw;
class BaseGameEntity;

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

public:
	LevelLayer* getCurrentLevel();

private:
	LevelLayer*					level_;
	std::shared_ptr<b2World>	world_;
	cocos2d::Label*				hit_point_label_;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
public:
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
protected:
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
private:
	cocos2d::CustomCommand custom_cmd_;
	std::auto_ptr<GLESDebugDraw> debug_draw_;
#endif
};

#endif