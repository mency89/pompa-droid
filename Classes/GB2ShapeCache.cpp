#include "GB2ShapeCache.h"

#include "Box2D/Box2D.h"
using namespace cocos2d;


/************************************************************************/
enum class FixtureType
{
	UNKNOWN,
	POLYGON,
	CIRCLE,
};

typedef std::vector<b2Vec2> Vertexs;

struct FixtureData
{
	float density;
	float friction;
	float restitution;

	int category_bits;
	int group_index;
	int mask_bits;

	bool is_sensor;
	std::string id;
	FixtureType type;

	union
	{
		struct
		{
			float radius;
			b2Vec2 position;
		};

		struct
		{
			std::vector<Vertexs> polygons;
		};
	};

	~FixtureData()
	{
		if (FixtureType::POLYGON != type)
		{
			memset(&polygons, 0, sizeof(std::vector<Vertexs>));
		}
	}
};

struct BodyDef
{
	b2Vec2 anchor_point;
	std::vector<FixtureData> fixtures;
};
/************************************************************************/


/************************************************************************/

const float GB2ShapeCache::s_PTMRatio_ = 32.0f;

GB2ShapeCache::GB2ShapeCache()
{

}

GB2ShapeCache::~GB2ShapeCache()
{

}

void GB2ShapeCache::removeAllShapes()
{
	body_shapes_.clear();
	shape_files_.clear();
}

void GB2ShapeCache::removeShapesWithFile(const std::string &plist)
{
	auto itr = shape_files_.find(plist);
	if (itr != shape_files_.end())
	{
		for (const auto &shape_name : itr->second)
		{
			body_shapes_.erase(shape_name);
		}
		shape_files_.erase(itr);
	}
}

bool GB2ShapeCache::addShapesWithFile(const std::string &plist)
{
	if (shape_files_.find(plist) != shape_files_.end())
	{
		return true;
	}

	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(plist);
	if (dict.empty())
	{
		return false;
	}

	ValueMap &meta_data = dict["metadata"].asValueMap();
	int format = meta_data["format"].asInt();
	if (format != 1)
	{
		CCASSERT(format == 1, "format not supported!");
		return false;
	}

	float PTM_Ratio_ = meta_data["ptm_ratio"].asFloat();
	CCAssert(PTM_Ratio_ == s_PTMRatio_, "error!");

	std::vector<std::string> shapes;
	ValueMap &body_dict = dict.at("bodies").asValueMap();
	for (auto iter = body_dict.cbegin(); iter != body_dict.cend(); ++iter)
	{
		const ValueMap &body_data = iter->second.asValueMap();
		body_shapes_.insert(std::make_pair(iter->first, std::auto_ptr<BodyDef>(new BodyDef())));
		BodyDef &body_def = *body_shapes_.find(iter->first)->second.get();

		Vec2 anchor_point = PointFromString(body_data.at("anchorpoint").asString());
		body_def.anchor_point = b2Vec2(anchor_point.x, anchor_point.y);
		const ValueVector &fixture_list = body_data.at("fixtures").asValueVector();
		for (auto &fixture_item : fixture_list)
		{
			body_def.fixtures.resize(body_def.fixtures.size() + 1);
			FixtureData &fd = body_def.fixtures.back();
			auto &fixture_data = fixture_item.asValueMap();

			fd.density = fixture_data.at("density").asFloat();
			fd.friction = fixture_data.at("friction").asFloat();
			fd.restitution = fixture_data.at("restitution").asFloat();

			fd.category_bits = fixture_data.at("filter_categoryBits").asInt();
			fd.group_index = fixture_data.at("filter_groupIndex").asInt();
			fd.mask_bits = fixture_data.at("filter_maskBits").asInt();

			fd.is_sensor = fixture_data.at("isSensor").asBool();
			fd.id = fixture_data.at("id").asString();

			std::string fixture_type = fixture_data.at("fixture_type").asString();
			if (fixture_type == "CIRCLE")
			{
				fd.type = FixtureType::CIRCLE;
				const ValueMap &circle_data = fixture_data.at("circle").asValueMap();
				fd.radius = circle_data.at("radius").asFloat() / s_PTMRatio_;
				Vec2 position = PointFromString(circle_data.at("position").asString()) / s_PTMRatio_;
				fd.position = b2Vec2(position.x, position.y);
			}
			else if (fixture_type == "POLYGON")
			{
				fd.type = FixtureType::POLYGON;
				const ValueVector &polygons_array = fixture_data.at("polygons").asValueVector();
				for (auto &polygon_item : polygons_array)
				{
					fd.polygons.resize(fd.polygons.size() + 1);
					Vertexs &polygon = fd.polygons.back();
					auto &polygon_array = polygon_item.asValueVector();
					for (auto &point_string : polygon_array)
					{
						auto offset = PointFromString(point_string.asString());
						polygon.push_back(b2Vec2(offset.x / s_PTMRatio_, offset.y / s_PTMRatio_));
					}
				}
			}
			else
			{
				fd.type = FixtureType::UNKNOWN;
				CCAssert(false, "UNKNOWN");
				return false;
			}
		}
		shapes.push_back(iter->first);
	}

	shape_files_.insert(std::make_pair(plist, std::move(shapes)));

	return true;
}

bool GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape, bool flipped)
{
	CCAssert(body, "nullptr");

	auto itr = body_shapes_.find(shape);
	if (itr != body_shapes_.end())
	{
		for (auto &fixture_item : itr->second->fixtures)
		{
			b2FixtureDef fixture;
			fixture.density = fixture_item.density;
			fixture.friction = fixture_item.friction;
			fixture.restitution = fixture_item.restitution;
			fixture.isSensor = fixture_item.is_sensor;

			b2Filter filter;
			filter.categoryBits = fixture_item.category_bits;
			filter.groupIndex = fixture_item.group_index;
			filter.maskBits = fixture_item.mask_bits;
			fixture.filter = filter;

			if (fixture_item.type == FixtureType::POLYGON)
			{
				for (auto vertexs : fixture_item.polygons)
				{
					// 翻转
					if (flipped)
					{
						for (size_t i = 0; i < vertexs.size(); ++i)
						{
							vertexs[i].x = -vertexs[i].x;
						}
					}

					b2PolygonShape polygon;
					polygon.Set(&vertexs[0], vertexs.size());
					fixture.shape = &polygon;
					body->CreateFixture(&fixture);
				}
			}
			else if (fixture_item.type == FixtureType::CIRCLE)
			{
				b2CircleShape circle;
				circle.m_p = fixture_item.position;
				circle.m_radius = fixture_item.radius;
				fixture.shape = &circle;
				body->CreateFixture(&fixture);
			}
			else
			{
				continue;
			}
		}

		return true;
	}
	return false;
}

bool GB2ShapeCache::anchorPointForShape(const std::string &shape, cocos2d::Vec2 &anchor_point)
{
	auto itr = body_shapes_.find(shape);
	if (itr != body_shapes_.end())
	{
		anchor_point.x = itr->second->anchor_point.x;
		anchor_point.y = itr->second->anchor_point.y;
		return true;
	}
	return false;
}
/************************************************************************/