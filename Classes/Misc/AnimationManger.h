#ifndef __ANIMATIONMANAGER_H__
#define __ANIMATIONMANAGER_H__

#include <unordered_map>
#include "Singleton.h"

namespace cocos2d
{
	class Animation;
}

class AnimationManger : public Singleton < AnimationManger >
{
	SINGLETON_DEFAULT(AnimationManger);

public:
	void addAnimationWithFile(const std::string &json);

	cocos2d::Animation* getAnimation(const std::string &name);

	void removeAnimation(const std::string &name);

	void removeAnimationWithFile(const std::string &json);

public:
	std::unordered_map<std::string, cocos2d::Animation*> animations_;
	std::unordered_map<std::string, std::vector<std::string> > loaded_files_;
};

#endif