#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "LevelLayer.h"
#include "VisibleRect.h"
#include "GLES-Render.h"
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
	if (level_->getHeroEntity())
	{
		Message msg;
		STKeyPressed extra_info;
		extra_info.key_code = keyCode;
		msg.receiver = level_->getHeroEntity()->getID();
		msg.msg_code = MessageTypes::kMsgKeyPressed;
		msg.extra_info = &extra_info;
		msg.extra_info_size = sizeof(STKeyPressed);
		MessageDispatcher::instance()->dispatchMessage(msg);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (level_->getHeroEntity() != nullptr)
	{
		Message msg;
		STKeyReleased extra_info;
		extra_info.key_code = keyCode;
		msg.receiver = level_->getHeroEntity()->getID();
		msg.msg_code = MessageTypes::kMsgKeyReleased;
		msg.extra_info = &extra_info;
		msg.extra_info_size = sizeof(STKeyReleased);
		MessageDispatcher::instance()->dispatchMessage(msg);
	}
}