#ifndef __TELEGRAM_H__
#define __TELEGRAM_H__

struct Telegram
{
	int sender;
	int receiver;
	int msg_code;
	float dispatch_time;
	void* extra_info;
	size_t size;

	Telegram()
		: sender(0)
		, receiver(0)
		, msg_code(0)
		, dispatch_time(0.0f)
		, extra_info(nullptr)
		, size(0)
	{

	}
};

#endif