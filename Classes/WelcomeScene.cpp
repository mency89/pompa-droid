#include "WelcomeScene.h"

#include "GameScene.h"
#include "VisibleRect.h"
using namespace cocos2d;


Scene* WelcomeScene::createScene()
{
    auto scene = Scene::create();
	auto layer = WelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	auto backgroup = Sprite::create("images/bg_title.png");
	backgroup->setPosition(VisibleRect::center());
	addChild(backgroup);

	auto title = Sprite::create("images/txt_title.png");
	title->setPosition(Vec2(backgroup->getContentSize().width / 2, backgroup->getContentSize().height / 1.5f));
	backgroup->addChild(title);

	auto touchtostart = Sprite::create("images/txt_touchtostart.png");
	touchtostart->setPosition(Vec2(backgroup->getContentSize().width / 2, backgroup->getContentSize().height / 4));
	touchtostart->runAction(Repeat::create(Blink::create(0.8f, 1), -1));
	backgroup->addChild(touchtostart);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *event)->bool
	{
		if (touchtostart->getBoundingBox().containsPoint(touch->getLocation()))
		{
			CCLOG("start game");
			Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, GameScene::createScene()));
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}