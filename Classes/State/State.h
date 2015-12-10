#ifndef __BASESTATE_H__
#define __BASESTATE_H__


template <typename T>
class State
{
public:
	virtual void enter(T *object) = 0;

	virtual void exit(T *object) = 0;

	virtual void execute(T *object) = 0;
};

#endif