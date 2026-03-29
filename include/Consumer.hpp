#pragma once

#include "Message.hpp"
#include "SleeperWithPeriod.hpp"

#include <cstring>

class Consumer {
public:
	constexpr Consumer(char name, uint64_t frequency) noexcept : _name(name), _period(SleeperWithPeriod::PeriodInNs(frequency / sizeof(Message)))
	{
	}

	void Write(Message& message)
	{
		_sleeperWithPeriod.emplace(_period);

		memset(message.message, {}, sizeof(message.message)); // Some operation to prevent optimization
	}

private:
	char _name;
	int64_t _period;

	std::optional<SleeperWithPeriod> _sleeperWithPeriod;
};
