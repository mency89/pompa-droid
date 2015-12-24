#include "LevelLayer.h"
#include "VisibleRect.h"
#include "Entity/Hero.h"
#include "Entity/EntityManger.h"
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
			float x = data["x"].asFloat();
			float y = data["y"].asFloat();
			hero_->setPosition(Vec2(data["x"].asFloat() + Hero::RealWidth * hero_->getAnchorPoint().x,
				data["y"].asFloat() + Hero::RealHeight * hero_->getAnchorPoint().y));
			addChild(hero_, 10);
		}
	}
	return hero_;
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

// 镜头跟随主角
void LevelLayer::followHeroWithCamera()
{
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
}

// 主角是否在屏幕中
bool LevelLayer::HeroInInsideOfStage() const
{
	if (hero_ != nullptr)
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();
		if (world_pos.x >= Hero::RealWidth * hero_->getAnchorPoint().x &&
			world_pos.x <= win_size.width - Hero::RealWidth * (1.0f - hero_->getAnchorPoint().x) &&
			world_pos.y >= Hero::RealHeight * hero_->getAnchorPoint().y &&
			world_pos.y <= getTileSize().height * getFloorHeight() + Hero::RealHeight * (1.0f - hero_->getAnchorPoint().y))
		{
			return true;
		}
	}
	return false;
}

// 自动调整主角位置
void LevelLayer::adjustmentHeroPosition()
{
	if (hero_ != nullptr && !HeroInInsideOfStage())
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();
		if (world_pos.x < Hero::RealWidth * hero_->getAnchorPoint().x)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(Hero::RealWidth * hero_->getAnchorPoint().x,
				world_pos.y)));
		}

		if (world_pos.x > win_size.width - Hero::RealWidth * (1.0f - hero_->getAnchorPoint().x))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(win_size.width - Hero::RealWidth * (1.0f - hero_->getAnchorPoint().x),
				world_pos.y)));
		}

		if (world_pos.y < Hero::RealHeight * hero_->getAnchorPoint().y)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				Hero::RealHeight * hero_->getAnchorPoint().y)));
		}

		if (world_pos.y > getTileSize().height * getFloorHeight() + Hero::RealHeight * (1.0f - hero_->getAnchorPoint().y))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				getTileSize().height * getFloorHeight() + Hero::RealHeight * (1.0f - hero_->getAnchorPoint().y))));
		}
	}
}

void LevelLayer::adjustmentHeroPositionX()
{
	if (hero_ != nullptr && !HeroInInsideOfStage())
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();

		if (world_pos.x < Hero::RealWidth * hero_->getAnchorPoint().x)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(Hero::RealWidth * hero_->getAnchorPoint().x,
				world_pos.y)));
		}

		if (world_pos.x > win_size.width - Hero::RealWidth * (1.0f - hero_->getAnchorPoint().x))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(win_size.width - Hero::RealWidth * (1.0f - hero_->getAnchorPoint().x),
				world_pos.y)));
		}
	}
}

void LevelLayer::adjustmentHeroPositionY()
{
	if (hero_ != nullptr && !HeroInInsideOfStage())
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();

		if (world_pos.y < Hero::RealHeight * hero_->getAnchorPoint().y)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				Hero::RealHeight * hero_->getAnchorPoint().y)));
		}

		if (world_pos.y > getTileSize().height * getFloorHeight() + Hero::RealHeight * (1.0f - hero_->getAnchorPoint().y))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				getTileSize().height * getFloorHeight() + Hero::RealHeight * (1.0f - hero_->getAnchorPoint().y))));
		}
	}
}

// 设置跟随主角
void LevelLayer::setFollowHero(bool follow)
{
	follow_ = follow;
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

void LevelLayer::update(float delta)
{
	// 更新演员
	entity_manger_->update();

	// 镜头跟随
	followHeroWithCamera();
}