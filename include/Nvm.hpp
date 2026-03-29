#pragma once

#include "Message.hpp"
#include "SleeperWithPeriod.hpp"

#include <fstream>
#include <mutex>

#include <unistd.h>

class Nvm final {
public:
	explicit Nvm(uint64_t frequency) : _period(SleeperWithPeriod::PeriodInNs(frequency / sizeof(Message)))
	{
	}

	void Write(const Message& message)
	{
		_sleeperWithPeriod.emplace(_period);

		if (const std::lock_guard lg(_mtx); _fileStream.is_open()) {
			_fileStream << message.dateTime.time_since_epoch().count() << message.from << message.to;
			for (auto ele : message.message)
				_fileStream << ele;
			_fileStream << '\n';
		}
	}

	~Nvm()
	{
		const std::lock_guard lg(_mtx);
		_fileStream.close();
		sync();
	}

private:
	int64_t _period;

	std::ofstream _fileStream{"storage.data"};
	std::mutex _mtx;

	std::optional<SleeperWithPeriod> _sleeperWithPeriod;
};
