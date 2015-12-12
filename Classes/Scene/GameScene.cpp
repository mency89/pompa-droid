#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "VisibleRect.h"
#include "Entity/Hero.h"

#include "MeesageTypes.h"
#include "MessageDispatcher.h"
#include "Entity/BaseGameEntity.h"
using namespace cocos2d;


GameScene::GameScene()
	: hero_(nullptr)
{

}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	world_ = std::make_shared<b2World>(b2Vec2(0, 0));
	world_->SetContinuousPhysics(true);

	hero_ = GameEntityCreator<Hero>().create(world_);
	hero_->setPosition(VisibleRect::center());
	addChild(hero_);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	hero_->update();
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	Telegram msg;
	msg.receiver = hero_->get_id();
	msg.msg_code = MessageTypes::msg_KeyPressed;
	msg.extra_info = &keyCode;
	msg.extra_info_size = sizeof(EventKeyboard::KeyCode);
	MessageDispatcher::instance()->dispatchMessage(msg);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	Telegram msg;
	msg.receiver = hero_->get_id();
	msg.msg_code = MessageTypes::msg_KeyReleased;
	msg.extra_info = &keyCode;
	msg.extra_info_size = sizeof(EventKeyboard::KeyCode);
	MessageDispatcher::instance()->dispatchMessage(msg);
}