#include "LevelLayer.h"
#include "VisibleRect.h"
using namespace cocos2d;

LevelLayer::LevelLayer(const std::string &level_name)
	: follow_target_(nullptr)
	, innerstage_left_(VisibleRect::left().x + VisibleRect::getVisibleRect().size.width / 5.0f)
	, inner_stage_right_(VisibleRect::right().x - VisibleRect::getVisibleRect().size.width / 5.0f)
{

}

LevelLayer::~LevelLayer()
{

}

LevelLayer* LevelLayer::create(const std::string &level_name)
{
	LevelLayer *ret = new (std::nothrow) LevelLayer(level_name);
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
	scheduleUpdate();

	return true;
}

void LevelLayer::update(float delta)
{
	if (follow_target_ != nullptr)
	{
		float localx = getPositionX() + follow_target_->getPositionX();
		if (localx < innerstage_left_)
		{
			setPositionX(innerstage_left_ - follow_target_->getPositionX());
			if (getPositionX() > 0)
			{
				setPositionX(0);
			}
		}
		else if (localx > inner_stage_right_)
		{
			setPositionX(inner_stage_right_ - follow_target_->getPositionX());
			if (getPositionX() < Director::getInstance()->getWinSize().width - getContentSize().width)
			{
				setPositionX(Director::getInstance()->getWinSize().width - getContentSize().width);
			}
		}
	}
}

// 设置跟随目标
void LevelLayer::setFollowTarget(cocos2d::Node *target)
{
	follow_target_ = target;
}

// 载入关卡
void LevelLayer::loadLevel(const std::string &level_name)
{
	initWithTMXFile(level_name);
	setPosition(Vec2::ZERO);
	setAnchorPoint(Vec2::ZERO);
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