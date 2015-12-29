#ifndef __SHAPECATEGORY_H__
#define __SHAPECATEGORY_H__

/*
 * 与PhysicsEditor编辑器中分类一致
 * 碰撞筛选已在PhysicsEditor编辑器中设置
 */
enum ShapeCategory
{
	shape_hero_body = 0x0001,
	shape_hero_weapon = 0x0002,
	shape_enemy_body = 0x0004,
	shape_enemy_weapon = 0x0008,
};

#endif