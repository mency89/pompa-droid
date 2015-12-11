#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <memory>
#include "cocos2d.h"
#include "GameEntityCreator.h"

class b2Body;
class b2World;

class BaseGameEntity : public cocos2d::Sprite
{
public:
	enum Direction
	{
		Up,
		Down,
		Left,
		Right
	};

public:
	BaseGameEntity(std::shared_ptr<b2World> world);

	virtual ~BaseGameEntity();

	virtual bool init() override;

	virtual void update();

	void update_collision_body_by_spriteframe();

public:
	Direction getDirection() const;

	void setDirection(Direction direction);

private:
	std::shared_ptr<b2World>	world_;
	b2Body*						collision_body_;
	std::string					sprite_frame_name_;
	Direction					direction_;
};

#endif