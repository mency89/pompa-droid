#ifndef __HERO_H__
#define __HERO_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Weapon;

class Hero : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Hero> > HeroStateMachine;

public:
	Hero(int type, std::shared_ptr<b2World> world);

	~Hero();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 是否是跳跃状态
	 */
	bool isJumpingState() const;

	/**
	 * 获取有限状态机
	 */
	StateMachine<Hero>* getStateMachine();

	/**
	 * 是否有武器
	 */
	bool hasWeapon() const;

	/**
	 * 卸载武器
	 */
	Weapon* unloadWeapon();

	/**
	 * 装载武器
	 */
	void loadWeapon(Weapon *weapon);

	/**
	 * 获取武器
	 */
	cocos2d::Node* getWeaponSkin();

	/**
	 * 获取跳跃前的y值
	 */
	float getBeforeJumpPositionY();

	/**
	 * 获取攻击力
	 */
	virtual unsigned int getAttack() const;

	/**
	 * 获取奔跑攻击力
	 */
	virtual unsigned int getRunAttack() const;

	/**
	 * 获取跳跃攻击力
	 */
	virtual unsigned int getJumpAttack() const;

private:
	Weapon* weapon_;
	cocos2d::Sprite* weapon_skin_;
	HeroStateMachine state_machine_;
};

#endif