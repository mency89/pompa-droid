#include "MainScene.h"
#include "AppDelegate.h"
using namespace cocos2d;


MainScene::MainScene(const std::vector<std::string> &filelist)
{
	for (auto &filename : filelist)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
		ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
		ValueMap& framesDict = dict["frames"].asValueMap();
		for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
		{
			image_list_.push(iter->first);
		}
	}
	image_sum_ = image_list_.size();
}

MainScene::~MainScene()
{

}

MainScene* MainScene::create(const std::vector<std::string> &filelist)
{
	auto ret = new (std::nothrow) MainScene(filelist);
	if (ret != nullptr && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	return nullptr;
}

Scene* MainScene::createScene(const std::vector<std::string> &filelist)
{
	auto scene = Scene::create();
	auto layer = MainScene::create(filelist);
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	scheduleUpdate();

	auto s = Director::getInstance()->getWinSize();
	label_ = Label::createWithSystemFont("0/0", "Microsoft Yahei", 72);
	label_->setPosition(Vec2(s.width / 2, s.height / 2));
	addChild(label_);
    
    return true;
}

void MainScene::update(float delta)
{
	if (image_list_.empty())
	{
		unscheduleUpdate();
		label_->setString("Completed!");
	}
	else
	{
		auto sp = Sprite::createWithSpriteFrameName(image_list_.front());
		sp->setAnchorPoint(Vec2::ZERO);
		auto render = RenderTexture::create(sp->getContentSize().width, sp->getContentSize().height);
		render->setVisible(false);
		addChild(render, -1);

		render->begin();
		sp->visit();
		render->end();
		Director::getInstance()->getRenderer()->render();

		Image *image = render->newImage(true);
		image->saveToFile(image_list_.front(), false);

		image_list_.pop();
		removeChild(render, true);

		char buffer[256];
		sprintf(buffer, "%u/%u", image_sum_ - image_list_.size(), image_sum_);
		label_->setString(buffer);
	}
}