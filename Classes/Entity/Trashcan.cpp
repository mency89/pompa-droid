#include "Trashcan.h"
#include "ShapeCategory.h"
using namespace cocos2d;


Trashcan::Trashcan(std::shared_ptr<b2World> world)
	: BaseGameEntity(world)
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

}

Rect Trashcan::getRealRect() const
{
	const Size size(fullWidth(), fullHeight());
	Vec2 origin = getPosition() - Vec2(size.width * getAnchorPoint().x, size.height * getAnchorPoint().y);
	Rect ret(origin.x, origin.y, realWidth(), realHeight());
	return ret;
}

// 获取有限状态机
StateMachine<Trashcan>* Trashcan::getStateMachine()
{
	return state_machine_.get();
}