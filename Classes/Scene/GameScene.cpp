#include "GameScene.h"

#include <Box2D/Box2D.h>
#include "VisibleRect.h"
#include "Entity/Hero.h"

#include "GLES-Render.h"
#include "MeesageTypes.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "GB2ShapeCache.h"
#endif

#include "MessageDispatcher.h"
#include "Entity/BaseGameEntity.h"
using namespace cocos2d;


GameScene::GameScene()
	: hero_(nullptr)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	, debug_draw_(new GLESDebugDraw(GB2ShapeCache::instance()->getPTMRatio()))
#endif
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

	// 创建物理事件
	world_ = std::make_shared<b2World>(b2Vec2(0, 0));
	world_->SetAllowSleeping(false);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	world_->SetDebugDraw(debug_draw_.get());
	debug_draw_->SetFlags(b2Draw::e_shapeBit);
#endif

	// 创建玩家
	hero_ = GameEntityCreator<Hero>().create(world_);
	hero_->setPosition(VisibleRect::center());
	addChild(hero_, -1);

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

	hero_->update();
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