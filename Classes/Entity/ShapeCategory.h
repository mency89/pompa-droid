#ifndef __SHAPECATEGORY_H__
#define __SHAPECATEGORY_H__

/*
 * 与PhysicsEditor编辑器中分类一致
 * 碰撞筛选已在PhysicsEditor编辑器中设置
 */
enum ShapeCategory
{
	kHeroBodyShape = 0x0001,
	kHeroWeaponShape = 0x0002,
	kEnemyBodyShape = 0x0004,
	kEnemyWeaponShape = 0x0008,
	kTrashcanBodyShape = 0x0010,
};

#endif