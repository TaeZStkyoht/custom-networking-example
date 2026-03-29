#pragma once

#include <thread>

class SleeperWithPeriod final {
public:
	[[nodiscard]] explicit SleeperWithPeriod(int64_t periodInNs) : _periodInNs(periodInNs)
	{
	}

	SleeperWithPeriod(const SleeperWithPeriod&) = delete;
	SleeperWithPeriod& operator=(const SleeperWithPeriod&) = delete;

	~SleeperWithPeriod()
	{
		if (const auto sleepTime = _periodInNs - (GetTimeInNs() - _startTimeInNs); sleepTime > 0)
			std::this_thread::sleep_for(std::chrono::nanoseconds(sleepTime));
	}

	static constexpr int64_t PeriodInNs(auto frameRate) noexcept
	{
		return static_cast<int64_t>(1'000'000'000 / frameRate);
	}

private:
	static int64_t GetTimeInNs()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	int64_t _startTimeInNs = GetTimeInNs();
	int64_t _periodInNs;
};
