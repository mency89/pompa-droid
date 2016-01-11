#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "BaseGameEntity.h"
#include "State/StateMachine.h"

class Robot : public BaseGameEntity
{
	typedef std::auto_ptr<StateMachine<Robot> > RobotStateMachine;

public:
	Robot(std::shared_ptr<b2World> world);

	~Robot();

	virtual bool init() override;

	virtual void update() override;

	virtual const char* name() const override;

	virtual int weaponCategoryBits() const override;

	virtual void handleMenssage(const Message &msg) override;

public:
	/**
	 * 获取特效播放器
	 */
	cocos2d::Node* getEffectsPlayer();

	/**
	 * 获取有限状态机
	 */
	StateMachine<Robot>* getStateMachine();

private:
	cocos2d::Sprite* player_;
	RobotStateMachine state_machine_;
};

#endif