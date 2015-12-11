#include "Hero.h"


Hero::Hero(std::shared_ptr<b2World> world)
	: BaseGameEntity(world)
{

}

Hero::~Hero()
{

}

bool Hero::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	return true;
}

void Hero::update()
{

}