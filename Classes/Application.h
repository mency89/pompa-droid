#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "cocos2d.h"
#include "Singleton.h"

class Application : public Singleton < Application >
{
	SINGLETON_DEFAULT(Application);
};

#endif