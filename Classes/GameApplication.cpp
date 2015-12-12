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
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* unused_event)
	{
		setKeyPressed(keyCode);
	};
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* unused_event)
	{
		setKeyReleased(keyCode);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameApplication::update(float delta)
{
	MessageDispatcher::instance()->update(delta);
}

EventKeyboard::KeyCode GameApplication::takeKeyPressed()
{
	EventKeyboard::KeyCode key = pressed_;
	setKeyPressed(EventKeyboard::KeyCode::KEY_NONE);
	return key;
}

EventKeyboard::KeyCode GameApplication::takeKeyReleased()
{
	EventKeyboard::KeyCode key = released_;
	setKeyReleased(EventKeyboard::KeyCode::KEY_NONE);
	return key;
}

void GameApplication::setKeyPressed(EventKeyboard::KeyCode keyCode)
{
	pressed_ = keyCode;
}

void GameApplication::setKeyReleased(EventKeyboard::KeyCode keyCode)
{
	released_ = keyCode;
}