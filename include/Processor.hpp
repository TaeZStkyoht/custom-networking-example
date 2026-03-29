#pragma once

#include "Consumer.hpp"
#include "Nvm.hpp"
#include "Provider.hpp"

class Processor final {
public:
	Processor();

	void Start();

	std::string QueueStatus();

private:
	void Work(const std::stop_token& stopToken);
	size_t QueueCount() const;

	static constexpr uint64_t _frequencyOf1Gbit = 1 << 30;
	static constexpr uint64_t _frequencyOf1Mbit = 1 << 20;

	std::ofstream _fileStream{"result.csv"};

	std::priority_queue<Message, std::vector<Message>, Message::Compare> _messages;
	mutable std::mutex _mtxQueue;

	Nvm _nvm{8 * _frequencyOf1Gbit};

	Consumer _interfaceC{'C', 15 * _frequencyOf1Mbit};
	Consumer _interfaceD{'D', 45 * _frequencyOf1Mbit};

	Provider _interfaceA{'A', _frequencyOf1Gbit, {'C', 'D'}};
	Provider _interfaceB{'B', _frequencyOf1Gbit, {'C', 'D'}};
	Provider _interfaceE{'E', _frequencyOf1Gbit, {'C', 'D'}};

	std::jthread _worker;
};
