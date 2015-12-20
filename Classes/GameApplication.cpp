#include "GameApplication.h"
#include "MessageDispatcher.h"
using namespace cocos2d;


GameApplication::GameApplication()
	: scene_(nullptr)
{
	init();
	onEnter();
}

GameApplication::~GameApplication()
{
	onExit();
}

bool GameApplication::init()
{
	scheduleUpdate();

	return true;
}

void GameApplication::update(float delta)
{
	MessageDispatcher::instance()->update(delta);
}

GameScene* GameApplication::getGameScene()
{
	return scene_;
}

void GameApplication::setGameScene(GameScene *scene)
{
	scene_ = scene;
}