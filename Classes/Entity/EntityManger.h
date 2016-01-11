#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <set>
#include <memory>
#include <unordered_map>

class b2World;
class LevelLayer;
class BaseGameEntity;

enum EntityType
{
	kEntityHero,
	kEntityRobot,
	kEntityBoss,
	kEntityTrashcan,
};

class EntityManger
{
public:
	EntityManger(std::shared_ptr<b2World> world, LevelLayer *level);

	~EntityManger();

	/**
	 * 创建实例
	 * @param EntityType 实例类型
	 */
	BaseGameEntity* create(EntityType type);

	/**
	 * 通过id获取实例
	 * @param int 实例id
	 */
	BaseGameEntity* getEntityByID(int id);

	/**
	 * 获取所有实例
	 */
	std::vector<BaseGameEntity*> getAllEntitys();

	/**
	 * 销毁实例
	 * @param int 实例id
	 */
	void destroyEntity(int id);

	/**
	 * 销毁实例
	 * @param BaseGameEntity* 实例指针
	 */
	void destroyEntity(BaseGameEntity *entity);

	/**
	 * 销毁所有实例
	 */
	void destroyAllEntity();

	/**
	 * 获取当前关卡
	 */
	LevelLayer* getCurrentLevel();

public:
	/**
	 * 用于更新实例状态
	 */
	void update();

private:
	LevelLayer* current_level_;
	std::set<int> destroy_set_;
	std::shared_ptr<b2World> world_;
	std::unordered_map<int, BaseGameEntity*> entitys_;
};

#endif