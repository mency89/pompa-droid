#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "State.h"

template <typename T>
struct StateMachineData
{
};

template <typename T>
class StateMachine
{
public:
	typedef T EntityType;

public:
	StateMachine(EntityType *owner)
		: owner_(owner)
		, current_state_(nullptr)
		, previous_state_(nullptr)
		, global_state_(nullptr)
	{

	}

	void set_current_state(State<EntityType> *new_state)
	{
		current_state_ = new_state;
	}

	void set_previous_state(State<EntityType> *new_state)
	{
		previous_state_ = new_state;
	}

	void set_global_state(State<EntityType> *new_state)
	{
		global_state_ = new_state;
	}

	void update() const
	{
		if (global_state_ != nullptr)
		{
			global_state_->execute(owner_);
		}

		if (current_state_ != nullptr)
		{
			current_state_->execute(owner_);
		}
	}

	void change_state(State<EntityType> *new_state)
	{
		if (new_state != nullptr)
		{
			previous_state_ = current_state_;
			if (current_state_ != nullptr)
			{
				current_state_->exit(owner_);
			}
			current_state_ = new_state;
			current_state_->enter(owner_);
		}
	}

	void revert_to_previous_state()
	{
		change_state(previous_state_);
	}

	State<EntityType>* get_current_state()
	{
		return current_state_;
	}

	State<EntityType>* get_previous_state()
	{
		return previous_state_;
	}

	State<EntityType>* get_global_state()
	{
		return global_state_;
	}

	bool handle_message(const Telegram &msg)
	{
		if (current_state_ != nullptr && current_state_->on_message(owner_, msg))
		{
			return true;
		}

		if (global_state_ != nullptr && global_state_->on_message(owner_, msg))
		{
			return true;
		}

		return false;
	}

	StateMachineData<EntityType>& userdata()
	{
		return userdata_;
	}

private:
	EntityType* const				owner_;
	State<EntityType>*				current_state_;
	State<EntityType>*				previous_state_;
	State<EntityType>*				global_state_;
	StateMachineData<EntityType>	userdata_;
};

#endif