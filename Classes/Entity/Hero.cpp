#include "Hero.h"
#include "ShapeCategory.h"
#include "State/HeroOwnedStates.h"
using namespace cocos2d;


Hero::Hero(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
	, has_weapon_(false)
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

	weapon_ = Sprite::create();
	weapon_->setAnchorPoint(Vec2::ZERO);
	addChild(weapon_);

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
	return has_weapon_;
}

// 设置携带武器
void Hero::setCrryWeapon(bool value)
{
	has_weapon_ = value;
}

// 获取武器
Node* Hero::getWeapon()
{
	weapon_->setFlippedX(isFlippedX());
	return weapon_;
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