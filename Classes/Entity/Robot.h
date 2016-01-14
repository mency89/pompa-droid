#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Robot : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Robot> > RobotStateMachine;

public:
	Robot(int type, std::shared_ptr<b2World> world);

	~Robot();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取腰带
	 */
	cocos2d::Node* getBelt();

	/**
	 * 获取烟雾
	 */
	cocos2d::Node* getSmoke();

	/**
	 * 获取有限状态机
	 */
	StateMachine<Robot>* getStateMachine();

private:
	cocos2d::Sprite* belt_;
	cocos2d::Sprite* smoke_;
	RobotStateMachine state_machine_;
};

#endif