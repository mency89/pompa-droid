#include "Hero.h"
#include "Weapon.h"
#include "ShapeCategory.h"
#include "State/HeroOwnedStates.h"
using namespace cocos2d;


Hero::Hero(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
	, weapon_(nullptr)
	, weapon_skin_(nullptr)
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

	weapon_skin_ = Sprite::create();
	weapon_skin_->setAnchorPoint(Vec2::ZERO);
	addChild(weapon_skin_);

	state_machine_.reset(new StateMachine<Hero>(this));
	state_machine_->set_global_state(HeroGlobal::instance());
	state_machine_->change_state(HeroIdle::instance());

	return true;
}

void Hero::update()
{
	state_machine_->update();
}

const char* Hero::name() const
{
	return "hero";
}

int Hero::weaponCategoryBits() const
{
	return kHeroWeaponShape;
}

void Hero::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 是否是跳跃状态
bool Hero::isJumpingState() const
{
	return state_machine_->get_current_state() == HeroJump::instance() ||
		state_machine_->get_current_state() == HeroJumpingAttack::instance();
}

// 获取有限状态机
StateMachine<Hero>* Hero::getStateMachine()
{
	return state_machine_.get();
}

// 是否有武器
bool Hero::hasWeapon() const
{
	return weapon_ != nullptr;
}

// 卸载武器
Weapon* Hero::unloadWeapon()
{
	if (hasWeapon())
	{
		Weapon *ret = weapon_;
		weapon_ = nullptr;
		ret->setLoad(false);
		return ret;
	}
	return nullptr;
}

// 装载武器
void Hero::loadWeapon(Weapon *weapon)
{
	CCAssert(weapon != nullptr, "");
	weapon_ = weapon;
	weapon_->setLoad(true);
}

// 获取武器
Node* Hero::getWeaponSkin()
{
	weapon_skin_->setFlippedX(isFlippedX());
	return weapon_skin_;
}

// 获取跳跃前的y值
float Hero::getBeforeJumpPositionY()
{
	return getStateMachine()->userdata().before_jump_y;
}

// 获取攻击力
unsigned int Hero::getAttack() const
{
	return hasWeapon() ? BaseGameEntity::getAttack() * 2 : BaseGameEntity::getAttack();
}

// 获取奔跑攻击力
unsigned int  Hero::getRunAttack() const
{
	return hasWeapon() ? BaseGameEntity::getRunAttack() * 2 : BaseGameEntity::getRunAttack();
}

// 获取跳跃攻击力
unsigned int  Hero::getJumpAttack() const
{
	return hasWeapon() ? BaseGameEntity::getJumpAttack() * 2 : BaseGameEntity::getJumpAttack();
}