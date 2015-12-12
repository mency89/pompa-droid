#ifndef __ENTITYNAMES_H__
#define __ENTITYNAMES_H__

#include <memory>
#include "cocos2d.h"
#include "Telegram.h"
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

	virtual void handleMenssage(const Telegram &msg);

	void update_collision_body_by_spriteframe();

public:
	int get_id() const;

	Direction getDirection() const;

	void setDirection(Direction direction);

private:
	int							entity_id_;
	std::shared_ptr<b2World>	world_;
	b2Body*						collision_body_;
	std::string					sprite_frame_name_;
	Direction					direction_;
	static int					s_next_entity_id_;
};

#endif