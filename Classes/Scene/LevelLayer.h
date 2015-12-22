#ifndef __LEVELLAYER_H__
#define __LEVELLAYER_H__

#include "cocos2d.h"

class LevelLayer : public cocos2d::Layer
{
public:
	LevelLayer(const std::string &level_name);

	~LevelLayer();

	virtual bool init() override;

	virtual void update(float delta) override;

	static LevelLayer* create(const std::string &level_name);

public:
	/**
	 * 载入关卡
	 * @param std::string 关卡名称
	 */
	void loadLevel(const std::string &level_name);

	/**
	 * 获取地板宽度
	 */
	int getFloorWidth() const;

	/**
	 * 获取地板高度
	 */
	int getFloorHeight() const;

	/**
	 * 设置跟随目标
	 */
	void setFollowTarget(cocos2d::Node *target);

private:
	std::string current_level_;
	const float inner_stage_x_;
	const float inner_stage_y_;
	cocos2d::Node* follow_target_;
	cocos2d::TMXTiledMap* tmx_layer_;
};

#endif