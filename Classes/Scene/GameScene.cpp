#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "VisibleRect.h"
#include "Entity/Hero.h"

#include "Entity/BaseGameEntity.h"
#include "Entity/GameEntityCreator.h"
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

	hero_ = GameEntityCreator<Hero>().create(nullptr);
	hero_->setPosition(VisibleRect::center());
	addChild(hero_);

	scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	hero_->update();
}