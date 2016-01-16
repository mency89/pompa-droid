#ifndef __LEVELLAYER_H__
#define __LEVELLAYER_H__

#include <memory>
#include "cocos2d.h"

class Hero;
class Weapon;
class b2World;
class EntityManger;
class BaseGameEntity;

class LevelLayer : public cocos2d::TMXTiledMap
{
	struct STrigger
	{
		struct SEntity
		{
			int direction;
			int entity_type;
			int num;
		};

		float x;
		float y;
		float width;
		float height;
		std::vector<SEntity> creater_;
	};

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
	Hero* getHero();

	/**
	 * 设置跟随主角
	 */
	void setFollowHero(bool follow);

	/**
	 * 自动调整位置
	 */
	void adjustmentPosition(BaseGameEntity *entity);
	void adjustmentPositionX(BaseGameEntity *entity);
	void adjustmentPositionY(BaseGameEntity *entity);

	/**
	 * 避开障碍物
	 */
	void trashcanAvoidance(BaseGameEntity *entity);

	/**
	 * 播放受击特效
	 */
	void playHitEffect(const cocos2d::Vec2 &local_pos, unsigned short hurtOfValue);

	/**
	 * 图层数量
	 */
	int layerCount() const;

	/**
	 * 拾取武器
	 */
	bool pickUpWeapon(Hero *hero);

	/**
	 * 掉落武器
	 */
	void dropWeapon(Hero *hero);

	/**
	 * 掉落武器
	 */
	void dropWeapon(const cocos2d::Vec2 &local_pos);

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
	void setRealEntityPosition(BaseGameEntity *entity, const cocos2d::Vec2 &world_pos);

private:
	/**
	 * 加载触发器
	 */
	void loadTriggers();

	/**
	 * 创建障碍物
	 */
	void createTrashcan();

	/**
	 * 获取图层数量
	 */
	int getLayerCount() const;

	/**
	 * 获取地板高度
	 */
	int getFloorHeight() const;

	/**
	 * 更新触发器状态
	 */
	void updateTruggersState();

private:
	bool							follow_;
	int								layer_count_;
	int								floor_height_;
	Hero*							hero_;
	const float						innerstage_left_;
	const float						inner_stage_right_;
	std::vector<STrigger>			triggers_;
	std::vector<cocos2d::Label*>	label_queue_;
	std::vector<cocos2d::Sprite*>	sprite_queue_;
	std::shared_ptr<b2World>		world_;
	std::auto_ptr<EntityManger>		entity_manger_;
	
};

#endif