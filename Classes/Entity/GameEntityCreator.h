#ifndef __GAMEENTITYCREATOR_H__
#define __GAMEENTITYCREATOR_H__


template <typename GameEntity>
struct GameEntityCreator
{
	static GameEntity* create(std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(world);
		if (entity && entity->init())
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}

	static GameEntity* create(const std::string &filename, std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(world);
		if (entity && entity->initWithFile(filename))
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}

	static GameEntity* create(const std::string &filename, const cocos2d::Rect &rect, std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(world);
		if (entity && entity->initWithFile(filename, rect))
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}

	static GameEntity* createWithTexture(cocos2d::Texture2D *texture, std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(world);
		if (entity && entity->initWithTexture(texture))
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}

	static GameEntity* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, std::shared_ptr<b2World> world)
	{
		GameEntity *entity = new (std::nothrow) GameEntity(world);
		if (entity && spriteFrame && entity->initWithSpriteFrame(spriteFrame))
		{
			entity->autorelease();
			return entity;
		}
		CC_SAFE_DELETE(entity);
		return nullptr;
	}

	static GameEntity* createWithSpriteFrameName(const std::string &spriteFrameName, std::shared_ptr<b2World> world)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
#if COCOS2D_DEBUG > 0
		char msg[256] = { 0 };
		sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
		CCASSERT(frame != nullptr, msg);
#endif
		return createWithSpriteFrame(frame, world);
	}
};

#endif