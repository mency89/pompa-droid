﻿#include "LevelLayer.h"
#include "VisibleRect.h"
#include "Entity/Hero.h"
#include "json/document.h"
#include "Entity/EntityManger.h"
using namespace cocos2d;

enum Direction
{
	Left,
	Right,
};

#define HERO_MAX_Y_CORRECTION 10

static const std::map<std::string, EntityType> gStringToEntityType = {
	{ "boss" , EntityType::kEntityBoss },
	{ "robot", EntityType::kEntityRobot },
};

LevelLayer::LevelLayer(std::shared_ptr<b2World> world, const std::string &level_name)
	: world_(world)
	, hero_(nullptr)
	, follow_(false)
	, layer_count_(0)
	, floor_height_(0)
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
	// 获取图层数量
	layer_count_ = getLayerCount();

	// 获取地板高度
	floor_height_ = getFloorHeight();

	// 创建游戏实例
	entity_manger_.reset(new EntityManger(world_, this));
	hero_ = getHeroEntity();

	// 加载触发器
	loadTriggers();

	// 创建障碍物
	//createTrashcan();

	scheduleUpdate();

	return true;
}

// 载入关卡
void LevelLayer::loadLevel(const std::string &level_name)
{
	initWithTMXFile(level_name);
	hero_ = nullptr;
	setPosition(Vec2::ZERO);
	setAnchorPoint(Vec2::ZERO);

	// 加载触发器
	loadTriggers();

	// 创建障碍物
	createTrashcan();

	// 获取图层数量
	layer_count_ = getLayerCount();

	// 获取地板高度
	floor_height_ = getFloorHeight();

	// 销毁所有实例
	entity_manger_->destroyAllEntity();
}

// 获取主角实例
Hero* LevelLayer::getHeroEntity()
{
	if (hero_ == nullptr)
	{
		auto objects = getObjectGroup("objects");
		if (objects != nullptr)
		{
			ValueMap data = objects->getObject("hero");
			float x = data["x"].asFloat();
			float y = data["y"].asFloat();
			hero_ = dynamic_cast<Hero *>(entity_manger_->create(kEntityHero));
			CCAssert(hero_ != nullptr, "");
			hero_->setAnchorPoint(Vec2(0.5f, 0.0f));
			setRealEntityPosition(hero_, Vec2(x, y));
		}
	}
	return hero_;
}

// 地板宽度
int LevelLayer::floorWidth() const
{
	auto floor_layer = getLayer("floor");
	if (floor_layer != nullptr)
	{
		return floor_layer->getLayerSize().width;
	}
	return 0;
}

// 地板高度
int LevelLayer::floorHeight() const
{
	return floor_height_;
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

// 加载触发器
void LevelLayer::loadTriggers()
{
	triggers_.clear();
	auto objects = getObjectGroup("objects");
	if (objects != nullptr)
	{
		for (auto value : objects->getObjects())
		{
			if (value.asValueMap()["type"].asString() == "trigger")
			{
				auto unpack = [](Direction direction, const std::string &json_string)
				{
					rapidjson::Document doc;
					std::vector<STrigger::SEntity> ret;
					doc.Parse<0>(json_string.c_str());
					for (size_t i = 0; i < doc.Size(); ++i)
					{
						const rapidjson::Value &object = doc[i];
						auto result = gStringToEntityType.find(object.MemberBegin()->name.GetString());
						CCAssert(result != gStringToEntityType.end(), "");
						if (result != gStringToEntityType.end())
						{
							STrigger::SEntity enyity;
							enyity.direction = direction;
							enyity.entity_type = result->second;
							enyity.num = object.MemberBegin()->value.GetInt();
							ret.push_back(enyity);
						}
						else
						{
							CCAssert(false, "");
						}
					}
					return ret;
				};

				STrigger trigger;
				trigger.x = value.asValueMap()["x"].asFloat();
				trigger.y = value.asValueMap()["y"].asFloat();
				trigger.width = value.asValueMap()["width"].asFloat();
				trigger.height = value.asValueMap()["height"].asFloat();
				auto vec = unpack(Direction::Left, value.asValueMap()["left"].asString());
				trigger.creater_.insert(trigger.creater_.end(), vec.begin(), vec.end());
				vec = unpack(Direction::Right, value.asValueMap()["right"].asString());
				trigger.creater_.insert(trigger.creater_.end(), vec.begin(), vec.end());
				triggers_.push_back(std::move(trigger));
			}
		}
	}
}

// 创建障碍物
void LevelLayer::createTrashcan()
{
	auto objects = getObjectGroup("objects");
	if (objects != nullptr)
	{
		for (auto value : objects->getObjects())
		{
			if (value.asValueMap()["type"].asString() == "trashcan")
			{
				float x = value.asValueMap()["x"].asFloat();
				float y = value.asValueMap()["y"].asFloat();
				auto entity = entity_manger_->create(EntityType::kEntityTrashcan);
				entity->setAnchorPoint(Vec2(0.5f, 0.0f));
				setRealEntityPosition(entity, Vec2(x, y));
			}
		}
	}
}

// 图层数量
int LevelLayer::layerCount() const
{
	return layer_count_;
}

// 获取图层数量
int LevelLayer::getLayerCount() const
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

// 获取非透明区域在世界中的坐标
Vec2 LevelLayer::getRealEntityPosition(BaseGameEntity *entity) const
{
	CCAssert(entity != nullptr && entity->getParent() == this, "");
	const Size size(entity->fullWidth(), entity->fullHeight());
	const Size real_size(entity->realWidth(), entity->realHeight());
	Vec2 origin = entity->getPosition() - Vec2(size.width * entity->getAnchorPoint().x,
		size.height * entity->getAnchorPoint().y);
	Vec2 real_origin = origin + Vec2(size.width / 2 - real_size.width / 2, size.height / 2 - real_size.height / 2);
	Vec2 real_pos(real_origin.x + real_size.width * entity->getAnchorPoint().x,
		real_origin.y + real_size.height * entity->getAnchorPoint().y);
	return convertToWorldSpace(real_pos);
}

// 设置非透明区域在世界中的坐标
void LevelLayer::setRealEntityPosition(BaseGameEntity *entity, const Vec2 &pos)
{
	CCAssert(entity != nullptr && entity->getParent() == this, "");
	Vec2 real_pos = convertToNodeSpace(pos);
	const Size size(entity->fullWidth(), entity->fullHeight());
	const Size real_size(entity->realWidth(), entity->realHeight());
	Vec2 real_origin = real_pos - Vec2(real_size.width * entity->getAnchorPoint().x, real_size.height * entity->getAnchorPoint().y);
	Vec2 origin = real_origin - Vec2(size.width / 2 - real_size.width / 2, size.height / 2 - real_size.height / 2);
	entity->setPosition(origin + Vec2(size.width * entity->getAnchorPoint().x, size.height * entity->getAnchorPoint().y));
}

// 获取真实矩形框（去除透明区域的矩形框）
Rect LevelLayer::getRealEntityRect(BaseGameEntity *entity) const
{
	Rect ret;
	if (entity != nullptr)
	{
		CCAssert(entity->getParent() == this, "");
		ret = entity->getRealRect();
		ret.origin = convertToWorldSpace(ret.origin);
	}
	return ret;
}

// 是否在地板内
bool LevelLayer::insideOfFloor(BaseGameEntity *entity) const
{
	if (entity != nullptr)
	{
		const Rect rect = getRealEntityRect(entity);
		Size win_size = Director::getInstance()->getWinSize();
		if (rect.getMinX() >= 0 &&
			rect.getMinY() >= 0 &&
			rect.getMaxX() <= win_size.width &&
			rect.getMinY() <= getTileSize().height * floorHeight() - HERO_MAX_Y_CORRECTION)
		{
			return true;
		}
	}
	return false;
}

// 是否相邻
bool LevelLayer::isAdjacent(BaseGameEntity *a, BaseGameEntity *b)
{
	CCAssert(a != nullptr && b != nullptr, "");
	Vec2 pos_a = convertToNodeSpace(getRealEntityPosition(a));
	Vec2 pos_b = convertToNodeSpace(getRealEntityPosition(b));
	if (abs(pos_b.y - pos_a.y) < getMapSize().height)
	{
		return a->getRealRect().intersectsRect(b->getRealRect());
	}
	return false;
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

// 自动调整主角位置
void LevelLayer::adjustmentHeroPosition()
{
	if (hero_ != nullptr && !insideOfFloor(hero_))
	{
		const Rect rect = getRealEntityRect(hero_);
		const Vec2 realEntityPos = getRealEntityPosition(hero_);
		const Size win_size = Director::getInstance()->getWinSize();
		const Size real_size(hero_->realWidth(), hero_->realHeight());
		const float right_boundary = win_size.width - real_size.width * (1.0f - hero_->getAnchorPoint().x);
		const float top_boundary = getTileSize().height * floorHeight() + real_size.height * hero_->getAnchorPoint().y - HERO_MAX_Y_CORRECTION;

		if (rect.getMinX() < 0)
		{
			setRealEntityPosition(hero_, Vec2(real_size.width * hero_->getAnchorPoint().x, realEntityPos.y));
		}

		if (rect.getMaxX() > right_boundary)
		{
			setRealEntityPosition(hero_, Vec2(right_boundary, realEntityPos.y));
		}

		if (rect.getMinY() < 0)
		{
			setRealEntityPosition(hero_, Vec2(realEntityPos.x, real_size.height * hero_->getAnchorPoint().y));
		}

		if (rect.getMinY() > top_boundary)
		{
			setRealEntityPosition(hero_, Vec2(realEntityPos.x, top_boundary));
		}
	}
}

void LevelLayer::adjustmentHeroPositionX()
{
	if (hero_ != nullptr && !insideOfFloor(hero_))
	{
		const Rect rect = getRealEntityRect(hero_);
		const Vec2 realEntityPos = getRealEntityPosition(hero_);
		const Size win_size = Director::getInstance()->getWinSize();
		const Size real_size(hero_->realWidth(), hero_->realHeight());
		const float right_boundary = win_size.width - real_size.width * (1.0f - hero_->getAnchorPoint().x);
	
		if (rect.getMinX() < 0)
		{
			setRealEntityPosition(hero_, Vec2(real_size.width * hero_->getAnchorPoint().x, realEntityPos.y));
		}

		if (rect.getMaxX() > right_boundary)
		{
			setRealEntityPosition(hero_, Vec2(right_boundary, realEntityPos.y));
		}
	}
}

void LevelLayer::adjustmentHeroPositionY()
{
	if (hero_ != nullptr && !insideOfFloor(hero_))
	{
		const Rect rect = getRealEntityRect(hero_);
		const Vec2 realEntityPos = getRealEntityPosition(hero_);
		const Size real_size(hero_->realWidth(), hero_->realHeight());
		const float top_boundary = getTileSize().height * floorHeight() + real_size.height * hero_->getAnchorPoint().y - HERO_MAX_Y_CORRECTION;

		if (rect.getMinY() < 0)
		{
			setRealEntityPosition(hero_, Vec2(realEntityPos.x, real_size.height * hero_->getAnchorPoint().y));
		}

		if (rect.getMinY() > top_boundary)
		{
			setRealEntityPosition(hero_, Vec2(realEntityPos.x, top_boundary));
		}
	}
}

// 避开障碍物
void LevelLayer::trashcanAvoidance(BaseGameEntity *entity)
{
	Rect heroRet = entity->getRealRect();
	for (auto *entity : entity_manger_->getAllEntitys())
	{	
		if (strcmp(entity->name(), "Trashcan") == 0)
		{
			Rect entityRet = entity->getRealRect();
		}
	}
}

// 设置跟随主角
void LevelLayer::setFollowHero(bool follow)
{
	follow_ = follow;
}

// 更新触发器状态
void LevelLayer::updateTruggersState()
{
	if (hero_ != nullptr && !triggers_.empty())
	{
		Rect hero_rect(hero_->getRealRect());
		for (size_t i = 0; i < triggers_.size(); ++i)
		{
			Rect trigger(triggers_[i].x, triggers_[i].y, triggers_[i].width, triggers_[i].height);
			if (hero_rect.intersectsRect(trigger))
			{
				// 生成机器人
				std::vector<BaseGameEntity *> entitys;
				Size size = Director::getInstance()->getWinSize();
				for (auto &item : triggers_[i].creater_)
				{
					// 创建实例
					entitys.clear();
					for (int i = 0; i < item.num; ++i)
					{
						auto entity = entity_manger_->create(static_cast<EntityType>(item.entity_type));
						entity->setAnchorPoint(Vec2(0.5f, 0.0f));
						entitys.push_back(entity);
					}

					// 随机位置
					for (size_t i = 0; i < entitys.size(); ++i)
					{
						Vec2 pos;
						if (item.direction == Direction::Left)
						{
							pos.x = - entitys[i]->realWidth() / 2;
						}
						else
						{
							pos.x = size.width + entitys[i]->realWidth() / 2;
						}
						pos.y = rand() % int(getTileSize().height * floorHeight());
						pos = convertToNodeSpace(pos);
						setRealEntityPosition(entitys[i], pos);
					}
				}
				std::swap(triggers_[i], triggers_.back());
				triggers_.pop_back();
			}
		}
	}
}

void LevelLayer::update(float delta)
{
	// 更新演员
	entity_manger_->update();

	// 镜头跟随
	followHeroWithCamera();

	// 更新触发器
	updateTruggersState();

	// 更新层级
	std::vector<BaseGameEntity*> entitys = entity_manger_->getAllEntitys();
	for (size_t i = 0; i < entitys.size(); ++i)
	{
		Size size = Director::getInstance()->getWinSize();
		entitys[i]->setLocalZOrder(layerCount() + (size.height - getRealEntityPosition(entitys[i]).y));
	}
}