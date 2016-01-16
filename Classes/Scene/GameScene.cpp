#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "LevelLayer.h"
#include "VisibleRect.h"
#include "GLES-Render.h"
#include "WelcomeScene.h"
#include "GameApplication.h"
#include "Entity/Hero.h"
#include "Entity/BaseGameEntity.h"
#include "Message/MeesageTypes.h"
#include "Message/MessageDispatcher.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "GB2ShapeCache.h"
#endif
using namespace cocos2d;


GameScene::GameScene()
	: level_(nullptr)
	, hit_point_label_(nullptr)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	, debug_draw_(new GLESDebugDraw(GB2ShapeCache::instance()->getPTMRatio()))
#endif
{

}

GameScene::~GameScene()
{
	GameApplication::instance()->setGameScene(nullptr);
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

	// 创建物理世界
	world_ = std::make_shared<b2World>(b2Vec2(0, 0));
	world_->SetContinuousPhysics(true);
	world_->SetAllowSleeping(false);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	world_->SetDebugDraw(debug_draw_.get());
	//debug_draw_->SetFlags(b2Draw::e_shapeBit);
#endif

	// 创建关卡
	level_ = LevelLayer::create(world_, "level/map_level1.tmx");
	addChild(level_, -1);

	// 玩家血量
	hit_point_label_ = Label::createWithBMFont("fonts/hud.fnt", "");
	hit_point_label_->setAnchorPoint(Vec2(0.0f, 0.5f));
	hit_point_label_->setPosition(VisibleRect::leftTop() + Vec2(10, -40));
	hit_point_label_->setColor(Color3B(90, 240, 100));
	addChild(hit_point_label_);

	// 注册游戏场景
	GameApplication::instance()->setGameScene(this);

	// 注册触摸事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	// 更新物理世界
	int velocityIterations = 8;
	int positionIterations = 1;
	world_->Step(delta, velocityIterations, positionIterations);

	// 更新关卡环境
	level_->update(delta);

	// 更新角色血量
	if (level_->getHero() != nullptr)
	{
		char str[32];
		sprintf(str, "%u", level_->getHero()->getHitPoint());
		hit_point_label_->setString(str);

		if (level_->getHero()->getHitPoint() <= 40)
		{
			hit_point_label_->setColor(Color3B(255, 0, 0));
		}
		else if (level_->getHero()->getHitPoint() <= 120)
		{
			hit_point_label_->setColor(Color3B(250, 160, 0));
		}

		if (level_->getHero()->isDeath())
		{
			auto label = Label::createWithBMFont("fonts/hud.fnt", "GAME OVER");
			label->setPosition(VisibleRect::right());
			label->setPositionX(label->getPositionX() + label->getContentSize().width / 2);
			label->setColor(Color3B(255, 0, 0));
			addChild(label);

			label->runAction(Sequence::create(
				MoveTo::create(0.5f, VisibleRect::center()),
				DelayTime::create(0.8f),
				MoveTo::create(0.5f, VisibleRect::left() - Vec2(label->getContentSize().width / 2, 0)),
				CallFunc::create([]()
			{
				Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, WelcomeScene::createScene()));
			}),
				nullptr));
			unscheduleUpdate();
		}
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
	custom_cmd_.init(_globalZOrder, transform, flags);
	custom_cmd_.func = CC_CALLBACK_0(GameScene::onDraw, this, transform, flags);
	renderer->addCommand(&custom_cmd_);
}

void GameScene::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director *director = Director::getInstance();
	CCAssert(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	world_->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
#endif

LevelLayer* GameScene::getCurrentLevel()
{
	return level_;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (level_->getHero())
	{
		Message msg;
		STKeyPressed extra_info;
		extra_info.key_code = keyCode;
		msg.receiver = level_->getHero()->getID();
		msg.msg_code = MessageTypes::kMsgKeyPressed;
		msg.extra_info = &extra_info;
		msg.extra_info_size = sizeof(STKeyPressed);
		MessageDispatcher::instance()->dispatchMessage(msg);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (level_->getHero() != nullptr)
	{
		Message msg;
		STKeyReleased extra_info;
		extra_info.key_code = keyCode;
		msg.receiver = level_->getHero()->getID();
		msg.msg_code = MessageTypes::kMsgKeyReleased;
		msg.extra_info = &extra_info;
		msg.extra_info_size = sizeof(STKeyReleased);
		MessageDispatcher::instance()->dispatchMessage(msg);
	}
}