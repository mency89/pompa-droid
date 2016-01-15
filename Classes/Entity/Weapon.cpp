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

	state_machine_.reset(new StateMachine<Weapon>(this));

	return true;
}

void Weapon::update()
{
	state_machine_->update();
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
	state_machine_->handle_message(msg);
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

// 获取有限状态机
StateMachine<Weapon>* Weapon::getStateMachine()
{
	return state_machine_.get();
}