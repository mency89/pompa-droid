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
	BaseGameEntity(std::shared_ptr<b2World> world);

	virtual ~BaseGameEntity();

private:
	b2Body* collision_body_;
	std::shared_ptr<b2World> world_;
};

#endif