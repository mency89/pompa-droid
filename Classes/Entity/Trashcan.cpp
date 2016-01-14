#include "Trashcan.h"
#include "ShapeCategory.h"
using namespace cocos2d;


Trashcan::Trashcan(int type, std::shared_ptr<b2World> world)
	: BaseGameEntity(type, world)
{

}

Trashcan::~Trashcan()
{

}

bool Trashcan::init()
{
	if (!BaseGameEntity::init())
	{
		return false;
	}

	setSpriteFrame("trashcan.png");
	state_machine_.reset(new StateMachine<Trashcan>(this));

	return true;
}

void Trashcan::update()
{

}

const char* Trashcan::name() const
{
	return "trashcan";
}

int Trashcan::weaponCategoryBits() const
{
	return kTrashcanBodyShape;
}

void Trashcan::handleMenssage(const Message &msg)
{
	state_machine_->handle_message(msg);
}

// 获取有限状态机
StateMachine<Trashcan>* Trashcan::getStateMachine()
{
	return state_machine_.get();
}