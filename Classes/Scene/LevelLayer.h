#ifndef __LEVELLAYER_H__
#define __LEVELLAYER_H__

#include <memory>
#include "cocos2d.h"

class b2World;
class EntityManger;
class BaseGameEntity;

class LevelLayer : public cocos2d::TMXTiledMap
{
public:
	LevelLayer(std::shared_ptr<b2World> world, const std::string &level_name);

	~LevelLayer();

	virtual bool init() override;

	virtual void update(float delta) override;

	static LevelLayer* create(std::shared_ptr<b2World> world, const std::string &level_name);

public:
	/**
	 * 载入关卡
	 * @param std::string 关卡名称
	 */
	void loadLevel(const std::string &level_name);

	/**
	 * 地板宽度
	 */
	int floorWidth() const;

	/**
	 * 地板高度
	 */
	int floorHeight() const;

	/**
	 * 获取主角实例
	 */
	BaseGameEntity* getHeroEntity();

	/**
	 * 设置跟随主角
	 */
	void setFollowHero(bool follow);

	/**
	 * 自动调整主角位置
	 */
	void adjustmentHeroPosition();
	void adjustmentHeroPositionX();
	void adjustmentHeroPositionY();

	/**
	 * 图层数量
	 */
	int layerCount() const;

	/**
	 * 镜头跟随主角
	 */
	void followHeroWithCamera();

	/**
	 * 是否在地板内
	 */
	bool insideOfFloor(BaseGameEntity *entity) const;

	/**
	 * 是否相邻
	 */
	bool isAdjacent(BaseGameEntity *a, BaseGameEntity *b);

	/**
	 * 获取真实矩形框（去除透明区域的矩形框）
	 */
	cocos2d::Rect getRealEntityRect(BaseGameEntity *entity) const;

	/**
	 * 获取非透明区域在世界中的坐标
	 */
	cocos2d::Vec2 getRealEntityPosition(BaseGameEntity *entity) const;

	/**
	 * 设置非透明区域在世界中的坐标
	 */
	void setRealEntityPosition(BaseGameEntity *entity, const cocos2d::Vec2 &pos);

private:
	/**
	 * 获取图层数量
	 */
	int getLayerCount() const;

	/**
	 * 获取地板高度
	 */
	int getFloorHeight() const;

private:
	bool						follow_;
	int							layer_count_;
	int							floor_height_;
	BaseGameEntity*				hero_;
	const float					innerstage_left_;
	const float					inner_stage_right_;
	std::shared_ptr<b2World>	world_;
	std::auto_ptr<EntityManger>	entity_manger_;
	
};

#endif