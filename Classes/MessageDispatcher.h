#ifndef __MESSAGEDISPATCHER_H__
#define __MESSAGEDISPATCHER_H__

#include <vector>
#include <unordered_map>
#include "Telegram.h"
#include "Singleton.h"

class BaseGameEntity;

class MessageDispatcher : public Singleton < MessageDispatcher >
{
	SINGLETON_DEFAULT(MessageDispatcher);

public:
	bool registerEntity(BaseGameEntity *entity);

	bool unregisterEntity(BaseGameEntity *entity);

	void dispatchMessage(const Telegram &msg);

	void postMessage(const Telegram &msg);

public:
	void update(float delta);

private:
	std::vector<Telegram> message_queue_;
	std::unordered_map<int, BaseGameEntity *> entitys_;
};

#endif