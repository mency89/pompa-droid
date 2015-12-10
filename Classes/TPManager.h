#ifndef __TPMANAGER_H__
#define __TPMANAGER_H__

#include <queue>
#include <string>
#include <unordered_map>
#include "cocos2d.h"
#include "Singleton.h"

class TPManager : public Singleton < TPManager >
{
	typedef std::function<void(size_t idx, size_t total)> asyncLoadCallback;

	struct Task
	{
		asyncLoadCallback			callback;
		std::vector<std::string>	filelist;
	};

	SINGLETON(TPManager);

public:
	/**
	 * 加载
	 * @param category 分类名
	 */
	void loadCategory(const std::string &category);

	/**
	 * 加载
	 * @param category 分类列表
	 */
	void loadCategory(const std::vector<std::string> &category);

	/**
	 * 异步加载
	 * @param category 分类名
	 */
	void loadCategoryAsync(const std::string &category, const asyncLoadCallback &callback);

	/**
	 * 异步加载
	 * @param category 分类列表
	 */
	void loadCategoryAsync(const std::vector<std::string> &category, const asyncLoadCallback &callback);

	/**
	 * 卸载
	 * @param category 分类名
	 */
	void unloadCategory(const std::string &category);

	/**
	 * 卸载
	 * @param category 分类列表
	 */
	void unloadCategory(const std::vector<std::string> &category);

private:
	/**
	 * 加载图片
	 */
	void loadImage();

	/**
	 * 读取分类配置
	 */
	void loadCategoryConfigFile(const std::string &file_path);

	/**
	 * 获取目录下文件
	 * @param category 分类名
	 */
	void getListDirFiles(const std::string category, std::vector<std::string> &out);

private:
	bool working_;
	std::queue<Task> task_queue_;
	std::unordered_map<std::string, std::vector<std::string> > category_;
};

#endif