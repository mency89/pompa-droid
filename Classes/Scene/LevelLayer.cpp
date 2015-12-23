#include "LevelLayer.h"
#include "VisibleRect.h"
using namespace cocos2d;

LevelLayer::LevelLayer(const std::string &level_name)
	: current_level_(level_name)
	, tmx_layer_(nullptr)
	, follow_target_(nullptr)
	, inner_stage_x_(VisibleRect::left().x + 100)
	, inner_stage_y_(VisibleRect::right().x - 100)
{

}

LevelLayer::~LevelLayer()
{

}

LevelLayer* LevelLayer::create(const std::string &level_name)
{
	LevelLayer *ret = new (std::nothrow) LevelLayer(level_name);
	if (ret != nullptr && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool LevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	tmx_layer_ = TMXTiledMap::create(current_level_);
	addChild(tmx_layer_);

	scheduleUpdate();

	return true;
}

void LevelLayer::update(float delta)
{
	if (follow_target_ != nullptr)
	{
		float localx = tmx_layer_->getPositionX() + follow_target_->getPositionX();
		if (localx < inner_stage_x_)
		{
			tmx_layer_->setPositionX(inner_stage_x_ - follow_target_->getPositionX());
			if (tmx_layer_->getPositionX() > 0)
			{
				tmx_layer_->setPositionX(0);
			}
		}
		else if (localx > inner_stage_y_)
		{
			tmx_layer_->setPositionX(inner_stage_y_ - follow_target_->getPositionX());
			if (tmx_layer_->getPositionX() < Director::getInstance()->getWinSize().width - tmx_layer_->getContentSize().width)
			{
				tmx_layer_->setPositionX(0);
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
	CCAssert(tmx_layer_ != nullptr, "");
	tmx_layer_->initWithTMXFile(level_name);
	current_level_ = level_name;
}

// 获取地板宽度
int LevelLayer::getFloorWidth() const
{
	if (tmx_layer_ == nullptr)
	{
		auto floor_layer = tmx_layer_->getLayer("floor");
		if (floor_layer != nullptr)
		{
			return floor_layer->getLayerSize().width;
		}
	}
	return 0;
}

// 获取地板高度
int LevelLayer::getFloorHeight() const
{
	if (tmx_layer_ == nullptr)
	{
		auto floor_layer = tmx_layer_->getLayer("floor");
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
	}
	return 0;
}