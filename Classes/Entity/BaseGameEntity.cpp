#include "BaseGameEntity.h"


BaseGameEntity::BaseGameEntity(std::shared_ptr<b2World> world)
	: world_(world)
	, collision_body_(nullptr)
{
	CCAssert(world_ != nullptr, "");
}

BaseGameEntity::~BaseGameEntity()
{

}