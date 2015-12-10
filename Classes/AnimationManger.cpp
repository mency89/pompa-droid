#include "AnimationManger.h"

#include "cocos2d.h"
#include "json/document.h"
using namespace cocos2d;


void AnimationManger::addAnimationWithFile(const std::string &json)
{
	Data data = FileUtils::getInstance()->getDataFromFile(json);
	if (data.isNull())
	{
		CCLOG("Failed to open %s", json.c_str());
		return;
	}

	if (loaded_files_.find(json) != loaded_files_.end())
	{
		return;
	}

	rapidjson::Document doc;
	doc.Parse<0>(std::string((const char *)data.getBytes(), data.getSize()).c_str());
	CCAssert(!doc.HasParseError(), "");
	CCAssert(doc.IsObject(), "");

	auto &animations = loaded_files_[json];
	std::unordered_map<std::string, std::vector<std::string> > animation_groups;
	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
	{
		CCAssert(itr->value.IsArray(), "");
		CCAssert(itr->name.IsString(), "");
		auto &group = animation_groups[itr->name.GetString()];
		const rapidjson::Value &json_array = itr->value;
		for (size_t i = 0; i < json_array.Size(); ++i)
		{
			CCAssert(json_array[i].IsString(), "");
			group.push_back(json_array[i].GetString());
		}
		animations.push_back(itr->name.GetString());
	}

	for (auto &group : animation_groups)
	{
		Animation *new_animation = Animation::create();
		for (size_t i = 0; i < group.second.size(); ++i)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(group.second[i]);
			CCAssert(frame != nullptr, "");
			if (frame != nullptr)
			{
				new_animation->addSpriteFrame(frame);
			}	
		}

		if (!new_animation->getFrames().empty())
		{
			new_animation->setLoops(1);
			new_animation->setDelayPerUnit(0.1f);
			new_animation->setRestoreOriginalFrame(true);
			new_animation->retain();
			AnimationCache::getInstance()->addAnimation(new_animation, group.first);
			animations_.insert(std::make_pair(group.first, new_animation));
		}
	}
}

Animation* AnimationManger::getAnimation(const std::string &name)
{
	auto result = animations_.find(name);
	return result != animations_.end() ? result->second : nullptr;
}

void AnimationManger::removeAnimation(const std::string &name)
{
	auto result = animations_.find(name);
	if (result != animations_.end())
	{
		animations_.erase(result);
	}
}

void AnimationManger::removeAnimationWithFile(const std::string &json)
{
	auto result = loaded_files_.find(json);
	if (result != loaded_files_.end())
	{
		for (auto &name : result->second)
		{
			auto itr = animations_.find(name);
			if (itr != animations_.end())
			{
				itr->second->release();
				animations_.erase(itr);
			}
		}
		loaded_files_.erase(result);
	}
}