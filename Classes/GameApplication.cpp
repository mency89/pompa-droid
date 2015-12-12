#include "GameApplication.h"

#include "MessageDispatcher.h"
using namespace cocos2d;


GameApplication::GameApplication()
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