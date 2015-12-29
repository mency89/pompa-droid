#ifndef __BASESTATE_H__
#define __BASESTATE_H__

#include "Message/Message.h"

template <typename T>
class State
{
public:
	virtual void enter(T *object) = 0;

	virtual void exit(T *object) = 0;

	virtual void execute(T *object) = 0;

	virtual bool on_message(T *object, const Message &msg) = 0;
};

#endif