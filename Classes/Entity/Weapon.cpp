#include "Weapon.h"
#include "ShapeCategory.h"


Weapon::Weapon(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
	, loaded_(false)
{

}

Weapon::~Weapon()
{

}

bool Weapon::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	setSpriteFrame("weapon_unequipped.png");

	return true;
}

void Weapon::update()
{
}

const char* Weapon::name() const
{
	return "weapon";
}

int Weapon::weaponCategoryBits() const
{
	return ShapeCategory::kNone;
}

void Weapon::handleMenssage(const Message &msg)
{
}

// 是否被装载
bool Weapon::isLoaded() const
{
	return loaded_;
}

// 设置装载
void Weapon::setLoad(bool value)
{
	loaded_ = value;
	setVisible(!loaded_);
}