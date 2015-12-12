#include "MessageDispatcher.h"
#include "Entity/BaseGameEntity.h"


bool MessageDispatcher::registerEntity(BaseGameEntity *entity)
{
	CCAssert(entity != nullptr, "");
	if (entity != nullptr)
	{
		entitys_.insert(std::make_pair(entity->get_id(), entity));
		return true;
	}
	return false;
}

bool MessageDispatcher::unregisterEntity(BaseGameEntity *entity)
{
	CCAssert(entity != nullptr, "");
	if (entity != nullptr)
	{
		auto result = entitys_.find(entity->get_id());
		if (result != entitys_.end())
		{
			entitys_.erase(result);
			return true;
		}
	}
	return false;
}

void MessageDispatcher::dispatchMessage(const Telegram &msg)
{
	if (msg.dispatch_time > 0.0f)
	{
		message_queue_.push_back(msg);
	}
	else
	{
		auto result = entitys_.find(msg.receiver);
		if (result != entitys_.end())
		{
			result->second->handleMenssage(msg);
		}
	}
}

void MessageDispatcher::postMessage(const Telegram &msg)
{
	message_queue_.push_back(msg);
}

void MessageDispatcher::update(float delta)
{
	for (auto &msg : message_queue_)
	{
		msg.dispatch_time -= delta;
		if (msg.dispatch_time <= 0.0f)
		{
			msg.dispatch_time = 0.0f;
			dispatchMessage(msg);
		}
	}
}