#ifndef __GB2SHAPECACHE_H__
#define __GB2SHAPECACHE_H__

#include <unordered_map>
#include "cocos2d.h"
#include "Singleton.h"

class b2Body;
struct BodyDef;

class GB2ShapeCache final : public Singleton < GB2ShapeCache >
{
	SINGLETON(GB2ShapeCache);

public:
	void removeAllShapes();

	void removeShapesWithFile(const std::string &plist);

	bool addShapesWithFile(const std::string &plist);

	bool addFixturesToBody(b2Body *body, const std::string &shape);

	bool anchorPointForShape(const std::string &shape, cocos2d::Vec2 &anchor_point);

public:
	static float getPTMRatio() { return s_PTMRatio_; };

private:
	const static float s_PTMRatio_;
	std::unordered_map<std::string, std::auto_ptr<BodyDef> > body_shapes_;
	std::unordered_map<std::string, std::vector<std::string> > shape_files_;
};

#endif