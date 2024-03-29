﻿#include "TexturePacker.h"

#include <sstream>
#include "Helper.h"
#include "json/document.h"
using namespace cocos2d;


TexturePacker::TexturePacker()
	: working_(false)
{
	loadCategoryConfigFile("config/TexturePacker.json");
}

TexturePacker::~TexturePacker()
{

}

// 读取分类配置
void TexturePacker::loadCategoryConfigFile(const std::string &file_path)
{
	Data data = FileUtils::getInstance()->getDataFromFile(file_path);
	if (!data.isNull())
	{
		rapidjson::Document doc;
		std::string buf(reinterpret_cast<char *>(data.getBytes()), data.getSize());
		doc.Parse<0>(buf.c_str());
		if (!doc.HasParseError())
		{
			for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
			{
				std::vector<std::string> filelist;
				const rapidjson::Value &json_array = itr->value;
				for (size_t i = 0; i < json_array.Size(); ++i)
				{
					filelist.push_back(json_array[i].GetString());
				}
				category_.insert(std::make_pair(itr->name.GetString(), std::move(filelist)));
			}
		}
	}
}

// 获取目录下文件
void TexturePacker::getListDirFiles(const std::string category, std::vector<std::string> &out)
{
	std::set<std::string> files;
	for (auto &pair : category_)
	{
		if (pair.first.find(category) == 0)
		{
			for (auto &filename : pair.second)
			{
				out.push_back(filename);
			}
		}
	}
}

// 加载
void TexturePacker::loadCategory(const std::string &category)
{
	std::vector<std::string> files;
	getListDirFiles(category, files);

	for (auto &filename : files)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
		ValueMap dictionary = FileUtils::getInstance()->getValueMapFromFile(fullPath);
		ValueMap &framesDict = dictionary["frames"].asValueMap();
		for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(iter->first);
			PolygonInfo info = frame->getPolygonInfo();
			info.filename = iter->first;
			frame->setPolygonInfo(info);
		}
	}
}

// 加载
void TexturePacker::loadCategory(const std::vector<std::string> &category)
{
	std::vector<std::string> files;

	for (auto &dir : category)
	{
		getListDirFiles(dir, files);
	}

	for (auto &filename : files)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
		ValueMap dictionary = FileUtils::getInstance()->getValueMapFromFile(fullPath);
		ValueMap &framesDict = dictionary["frames"].asValueMap();
		for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(iter->first);
			PolygonInfo info = frame->getPolygonInfo();
			info.filename = iter->first;
			frame->setPolygonInfo(info);
		}
	}
}

// 异步加载
void TexturePacker::loadCategoryAsync(const std::string &category, const asyncLoadCallback &callback)
{
	Task task;
	task.callback = callback;
	getListDirFiles(category, task.filelist);
	if (!task.filelist.empty())
	{
		task_queue_.push(std::move(task));
		loadImage();
	}
}

// 异步加载
void TexturePacker::loadCategoryAsync(const std::vector<std::string> &category, const asyncLoadCallback &callback)
{
	Task task;
	task.callback = callback;

	for (auto &dir : category)
	{
		getListDirFiles(dir, task.filelist);
	}

	if (!task.filelist.empty())
	{
		task_queue_.push(std::move(task));
		loadImage();
	}
}

// 卸载
void TexturePacker::unloadCategory(const std::string &category)
{
	std::vector<std::string> files;
	getListDirFiles(category, files);

	for (auto &filename : files)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filename);
		Director::getInstance()->getTextureCache()->removeTextureForKey(Splitext(filename)[0] + ".png");
	}
}

// 卸载
void TexturePacker::unloadCategory(const std::vector<std::string> &category)
{
	std::vector<std::string> files;

	for (auto &dir : category)
	{
		getListDirFiles(dir, files);
	}

	for (auto &filename : files)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filename);
		Director::getInstance()->getTextureCache()->removeTextureForKey(Splitext(filename)[0] + ".png");
	}
}

// 加载图片
void TexturePacker::loadImage()
{
	if (!working_ && !task_queue_.empty())
	{
		int idx = 0;
		Task &task = task_queue_.front();
		for (auto &filename : task.filelist)
		{
			++idx;
			Director::getInstance()->getTextureCache()->addImageAsync(Splitext(filename)[0] + ".png", [&task, &filename, this, idx](Texture2D *texture)
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename, texture);
				std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
				ValueMap dictionary = FileUtils::getInstance()->getValueMapFromFile(fullPath);
				ValueMap &framesDict = dictionary["frames"].asValueMap();
				for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
				{
					auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(iter->first);
					PolygonInfo info = frame->getPolygonInfo();
					info.filename = iter->first;
					frame->setPolygonInfo(info);
				}

				task.callback(idx, task.filelist.size());
				if (idx == task.filelist.size())
				{
					task_queue_.pop();
					working_ = false;
					loadImage();
				}
			});
		}
	}
}