#ifndef __MESSAGE_H__
#define __MESSAGE_H__

struct Message
{
	int sender;					// 发送者
	int receiver;				// 接收者
	int msg_code;				// 消息类型
	float dispatch_time;		// 延迟时间
	const void* extra_info;		// 额外信息
	size_t extra_info_size;		// 额外信息大小

	Message()
		: sender(0)
		, receiver(0)
		, msg_code(0)
		, dispatch_time(0.0f)
		, extra_info(nullptr)
		, extra_info_size(0)
	{

	}
};

#endif