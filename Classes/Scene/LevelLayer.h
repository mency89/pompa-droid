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
	 * 获取地板宽度
	 */
	int getFloorWidth() const;

	/**
	 * 获取地板高度
	 */
	int getFloorHeight() const;

	/**
	 * 获取主角实例
	 */
	BaseGameEntity* getHeroEntity();

	/**
	 * 设置跟随主角
	 */
	void setFollowHero(bool follow);

	/**
	 * 目标是否在屏幕中
	 */
	bool isInInsideOfStage(cocos2d::Node *target);

private:
	bool						follow_;
	BaseGameEntity*				hero_;
	const float					innerstage_left_;
	const float					inner_stage_right_;
	std::shared_ptr<b2World>	world_;
	std::auto_ptr<EntityManger>	entity_manger_;
	
};

#endif