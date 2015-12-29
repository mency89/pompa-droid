#include "MessageDispatcher.h"
#include "Entity/BaseGameEntity.h"


bool MessageDispatcher::registerEntity(BaseGameEntity *entity)
{
	CCAssert(entity != nullptr, "");
	if (entity != nullptr)
	{
		entitys_.insert(std::make_pair(entity->getID(), entity));
		return true;
	}
	return false;
}

bool MessageDispatcher::unregisterEntity(BaseGameEntity *entity)
{
	CCAssert(entity != nullptr, "");
	if (entity != nullptr)
	{
		auto result = entitys_.find(entity->getID());
		if (result != entitys_.end())
		{
			entitys_.erase(result);
			return true;
		}
	}
	return false;
}

void MessageDispatcher::erase_extra_info(const void *extra_info, size_t size)
{
	size_t pos = (const char *)extra_info - extra_info_.data();
	if (pos + size <= extra_info_.size())
	{
		::memmove(extra_info_.data() + pos, extra_info_.data() + pos + size, size);
		extra_info_.resize(extra_info_.size() - size);
	}
	else
	{
		CCAssert(false, "");
	}
}

const void* MessageDispatcher::insert_extra_info(const void *extra_info, size_t size)
{
	size_t pos = extra_info_.size();
	extra_info_.resize(extra_info_.size() + size);
	::memcpy(extra_info_.data() + pos, extra_info, size);
	return extra_info_.data() + pos;
}

void MessageDispatcher::postMessage(const Telegram &msg)
{
	message_queue_.push_back(msg);
	if (msg.extra_info != nullptr && msg.extra_info_size > 0)
	{
		message_queue_.back().extra_info = insert_extra_info(msg.extra_info, msg.extra_info_size);
	}
}

void MessageDispatcher::dispatchMessage(const Telegram &msg)
{
	if (msg.dispatch_time > 0.0f)
	{
		postMessage(msg);
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

void MessageDispatcher::update(float delta)
{
	for (auto itr = message_queue_.begin(); itr != message_queue_.end();)
	{
		itr->dispatch_time -= delta;
		if (itr->dispatch_time <= 0.0f)
		{
			itr->dispatch_time = 0.0f;
			const void *extra_info = itr->extra_info;
			const size_t extra_info_size = itr->extra_info_size;
			dispatchMessage(*itr);
			if (extra_info != nullptr && extra_info_size > 0)
			{
				erase_extra_info(extra_info, extra_info_size);
			}	
			itr = message_queue_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}