#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "Entity/Hero.h"
#include "Entity/BaseGameEntity.h"
#include "Entity/GameEntityCreator.h"
using namespace cocos2d;


GameScene::GameScene()
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

	auto hero = GameEntityCreator<Hero>().create(nullptr);
	hero->setPosition(Vec2(200, 200));
	addChild(hero);

	return true;
}