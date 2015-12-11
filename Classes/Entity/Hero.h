#ifndef __HERO_H__
#define __HERO_H__

#include "BaseGameEntity.h"

class Hero : public BaseGameEntity
{
public:
	Hero(std::shared_ptr<b2World> world);

	~Hero();

	virtual bool init() override;

	virtual void update() override;

private:

};

#endif