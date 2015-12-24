#include "LevelLayer.h"
#include "VisibleRect.h"
#include "Entity/EntityManger.h"
#include "Entity/BaseGameEntity.h"
using namespace cocos2d;

LevelLayer::LevelLayer(std::shared_ptr<b2World> world, const std::string &level_name)
	: world_(world)
	, hero_(nullptr)
	, follow_(false)
	, innerstage_left_(VisibleRect::left().x + VisibleRect::getVisibleRect().size.width / 5.0f)
	, inner_stage_right_(VisibleRect::right().x - VisibleRect::getVisibleRect().size.width / 5.0f)
{

}

LevelLayer::~LevelLayer()
{

}

LevelLayer* LevelLayer::create(std::shared_ptr<b2World> world, const std::string &level_name)
{
	LevelLayer *ret = new (std::nothrow) LevelLayer(world, level_name);
	if (ret != nullptr && ret->initWithTMXFile(level_name))
	{
		ret->autorelease();
		ret->init();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool LevelLayer::init()
{
	// 创建游戏实例
	entity_manger_.reset(new EntityManger(world_));
	hero_ = getHeroEntity();
	setFollowHero(true);

	scheduleUpdate();

	return true;
}

void LevelLayer::update(float delta)
{
	// 更新演员
	entity_manger_->update();

	// 镜头跟随
	if (follow_)
	{
		BaseGameEntity *follow_target = getHeroEntity();
		if (follow_target != nullptr)
		{
			float localx = getPositionX() + follow_target->getPositionX();
			if (localx < innerstage_left_)
			{
				setPositionX(innerstage_left_ - follow_target->getPositionX());
				if (getPositionX() > 0)
				{
					setPositionX(0);
				}
			}
			else if (localx > inner_stage_right_)
			{
				setPositionX(inner_stage_right_ - follow_target->getPositionX());
				if (getPositionX() < Director::getInstance()->getWinSize().width - getContentSize().width)
				{
					setPositionX(Director::getInstance()->getWinSize().width - getContentSize().width);
				}
			}
		}
	}

	// 保证主角始终在舞台中
	if (!isInInsideOfStage(getHeroEntity()))
	{

	}
}

// 设置跟随主角
void LevelLayer::setFollowHero(bool follow)
{
	follow_ = follow;
}

// 获取主角实例
BaseGameEntity* LevelLayer::getHeroEntity()
{
	if (hero_ == nullptr)
	{
		auto objects = getObjectGroup("objects");
		if (objects != nullptr)
		{
			ValueMap data = objects->getObject("Hero");
			hero_ = entity_manger_->create(entity_hero);
			hero_->setPosition(Vec2(data["x"].asFloat(), data["y"].asFloat()));
			addChild(hero_, 10);
		}
	}
	return hero_;
}

// 载入关卡
void LevelLayer::loadLevel(const std::string &level_name)
{
	initWithTMXFile(level_name);
	hero_ = nullptr;
	setPosition(Vec2::ZERO);
	setAnchorPoint(Vec2::ZERO);
	entity_manger_->destroyAllEntity();
}

// 获取地板宽度
int LevelLayer::getFloorWidth() const
{
	auto floor_layer = getLayer("floor");
	if (floor_layer != nullptr)
	{
		return floor_layer->getLayerSize().width;
	}
	return 0;
}

// 获取地板高度
int LevelLayer::getFloorHeight() const
{
	auto floor_layer = getLayer("floor");
	if (floor_layer != nullptr)
	{
		int max_height = 0;
		const int width = floor_layer->getLayerSize().width;
		const int height = floor_layer->getLayerSize().height;
		for (int column = 0; column < width; ++column)
		{
			for (int row = 0; row < height; ++row)
			{
				int idx = row * width + column;
				if (floor_layer->getTiles()[idx] != 0)
				{
					if (height - row > max_height)
					{
						max_height = height - row;
					}
					break;
				}
			}
		}
		return max_height;
	}
	return 0;
}

// 目标是否在屏幕中
bool LevelLayer::isInInsideOfStage(Node *target)
{
	CCAssert(target != nullptr, "");
	if (target != nullptr)
	{
		Vec2 pos = target->getPosition();
		auto win_size = Director::getInstance()->getWinSize();
		if (pos.x >= target->getContentSize().width * target->getAnchorPoint().x &&
			pos.x <= win_size.width - target->getContentSize().width * (1.0f - target->getAnchorPoint().x) &&
			pos.y >= target->getContentSize().height * target->getAnchorPoint().y &&
			pos.y <= win_size.height - target->getContentSize().height * (1.0f - target->getAnchorPoint().y))
		{
			return true;
		}
	}
	return false;
}