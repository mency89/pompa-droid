#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
	MainScene(const std::vector<std::string> &string_list);

	~MainScene();

    virtual bool init() override;

	virtual void update(float delta) override;

	static MainScene* create(const std::vector<std::string> &filelist);

	static cocos2d::Scene* createScene(const std::vector<std::string> &filelist);

private:
	size_t image_sum_;
	cocos2d::Label* label_;
	std::queue<std::string> image_list_;
};

#endif
