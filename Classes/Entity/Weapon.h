#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Weapon : public BaseGameEntity
{
public:
	Weapon(int type, std::shared_ptr<b2World> world);

	~Weapon();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 是否被装载
	 */
	bool isLoaded() const;

	/**
	 * 设置装载
	 */
	void setLoad(bool value);

private:
	bool loaded_;
};

#endif