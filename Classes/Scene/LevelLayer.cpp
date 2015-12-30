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
			float x = data["x"].asFloat();
			float y = data["y"].asFloat();
			hero_ = entity_manger_->create(entity_hero);
			if (hero_ != nullptr)
			{
				hero_->setAnchorPoint(Vec2(0.5f, 0.0f));
				Vec2 offset = calculBornPositionOffset("Hero", hero_);
				hero_->setPosition(Vec2(x + offset.x, y + offset.y));
				addChild(hero_, layerCount());
			}
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


// 计算出生位置偏移
Vec2 LevelLayer::calculBornPositionOffset(const std::string &object_name, BaseGameEntity *entity) const
{
	if (entity != nullptr)
	{
		const Size size(entity->fullWidth(), entity->fullHeight());
		const Size real_size(entity->realWidth(), entity->realHeight());
		Vec2 origin = Vec2(size.width / 2 - real_size.width / 2, size.height / 2 - real_size.height / 2);
		Vec2 offset = Vec2(real_size.width * entity->getAnchorPoint().x, real_size.height * entity->getAnchorPoint().y);
		Vec2 node_pos = Vec2(size.width * entity->getAnchorPoint().x, size.height * entity->getAnchorPoint().y);
		return node_pos - (origin + offset);
	}
	return Vec2::ZERO;
}

// 图层数量
int LevelLayer::layerCount() const
{
	int count = 0;
	for (auto &child : getChildren())
	{
		TMXLayer *layer = dynamic_cast<TMXLayer *>(child);
		if (layer != nullptr)
		{
			++count;
		}
	}
	return count;
}

// 获取真实矩形框
Rect LevelLayer::getEntityRealRect(BaseGameEntity *entity) const
{
	Rect ret;
	if (entity != nullptr)
	{
		CCAssert(entity->getParent() == this, "");
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		const Size size(hero_->fullWidth(), hero_->fullHeight());
		const Size real_size(hero_->realWidth(), hero_->realHeight());
		Vec2 origin = world_pos - Vec2(size.width * hero_->getAnchorPoint().x, size.height * hero_->getAnchorPoint().y);
		Vec2 real_origin = origin + Vec2(size.width / 2 - real_size.width / 2, size.height / 2 - real_size.height / 2);
		ret.setRect(real_origin.x, real_origin.y, real_size.width, real_size.height);
	}
	return ret;
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

// 是否在地板内
bool LevelLayer::insideOfFloor(BaseGameEntity *entity) const
{
	if (entity != nullptr)
	{
		const Rect rect = getEntityRealRect(entity);
		Size win_size = Director::getInstance()->getWinSize();
		if (rect.getMinX() >= 0 &&
			rect.getMinY() >= 0 &&
			rect.getMaxX() <= win_size.width &&
			rect.getMinY() <= getTileSize().height * getFloorHeight())
		{
			return true;
		}
	}
	return false;
}

// 自动调整主角位置
void LevelLayer::adjustmentHeroPosition()
{
	if (hero_ != nullptr && !insideOfFloor(hero_))
	{
		const int floor_height = getFloorHeight();
		const Rect rect = getEntityRealRect(hero_);
		Size win_size = Director::getInstance()->getWinSize();
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		const Size size(hero_->fullWidth(), hero_->fullHeight());
		const Size real_size(hero_->realWidth(), hero_->realHeight());

		if (rect.getMinX() < 0)
		{

		}

		if (rect.getMaxX() > win_size.width - real_size.width * (1.0f - hero_->getAnchorPoint().x))
		{

		}

		if (rect.getMinY() < 0)
		{

		}

		if (rect.getMaxY() > getTileSize().height * floor_height + real_size.height * (1.0f - hero_->getAnchorPoint().y))
		{

		}
	}
}

void LevelLayer::adjustmentHeroPositionX()
{
	/*if (hero_ != nullptr && !HeroInInsideOfStage())
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();

		if (world_pos.x < Hero::realWidth() * hero_->getAnchorPoint().x)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(Hero::realWidth() * hero_->getAnchorPoint().x,
				world_pos.y)));
		}

		if (world_pos.x > win_size.width - Hero::realWidth() * (1.0f - hero_->getAnchorPoint().x))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(win_size.width - Hero::realWidth() * (1.0f - hero_->getAnchorPoint().x),
				world_pos.y)));
		}
	}*/
}

void LevelLayer::adjustmentHeroPositionY()
{
	/*if (hero_ != nullptr && !HeroInInsideOfStage())
	{
		Vec2 world_pos = convertToWorldSpace(hero_->getPosition());
		Size win_size = Director::getInstance()->getWinSize();

		if (world_pos.y < Hero::realHeight() * hero_->getAnchorPoint().y)
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				Hero::realHeight() * hero_->getAnchorPoint().y)));
		}

		if (world_pos.y > getTileSize().height * getFloorHeight() + Hero::realHeight() * (1.0f - hero_->getAnchorPoint().y))
		{
			hero_->setPosition(convertToNodeSpace(Vec2(world_pos.x,
				getTileSize().height * getFloorHeight() + Hero::realHeight() * (1.0f - hero_->getAnchorPoint().y))));
		}
	}*/
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