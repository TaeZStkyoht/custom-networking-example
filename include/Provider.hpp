#pragma once

#include "Message.hpp"
#include "SleeperWithPeriod.hpp"

#include <functional>
#include <mutex>
#include <queue>

class Provider {
public:
	Provider(char name, uint64_t frequency, std::vector<char> providingCandidates) noexcept
		: _name(name), _period(SleeperWithPeriod::PeriodInNs(frequency / sizeof(Message))), _providingCandidates(std::move(providingCandidates))
	{
	}

	void Start()
	{
		_sampler = std::jthread([this](const std::stop_token& stopToken) { Sample(stopToken); });
	}

	void Subscribe(std::function<void(const Message&)> callback)
	{
		_provider = std::jthread([this, callback = std::move(callback)](const std::stop_token& stopToken) { Provide(stopToken, std::move(callback)); });
	}

	size_t QueueCount() const
	{
		const std::lock_guard lg(_mtxQueue);
		return _messageQueue.size();
	}

private:
	void Sample(const std::stop_token& stopToken)
	{
		while (!stopToken.stop_requested()) {
			const SleeperWithPeriod sleeper(static_cast<int64_t>(static_cast<float>(_period) / _averageUtilityRateOfSampling));

			const Message message = {
				.dateTime = std::chrono::system_clock::now(),
				.from = _name,
				.to = _providingCandidates.at(rand() % _providingCandidates.size()),
				.message = {},
			};

			std::lock_guard lg(_mtxQueue);

			if (_messageQueue.size() >= 1'000'000)
				continue;

			_messageQueue.push(message);
		}
	}

	void Provide(const std::stop_token& stopToken, std::function<void(const Message&)> callback)
	{
		while (!stopToken.stop_requested()) {
			const SleeperWithPeriod sleeper(static_cast<int64_t>(static_cast<float>(_period) / _averageUtilityRateOfProviding));

			std::unique_lock uniqueLock(_mtxQueue);

			if (_messageQueue.empty())
				continue;

			const Message message = _messageQueue.front();
			_messageQueue.pop();
			uniqueLock.unlock();

			callback(message);
		}
	}

	static constexpr float _averageUtilityRateOfSampling = 0.003f;
	static constexpr float _averageUtilityRateOfProviding = 0.003f;

	char _name;
	int64_t _period;
	std::vector<char> _providingCandidates;

	std::queue<Message> _messageQueue;
	mutable std::mutex _mtxQueue;

	std::jthread _sampler;
	std::jthread _provider;
};
