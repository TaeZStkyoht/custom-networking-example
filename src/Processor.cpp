#include "Processor.hpp"

#include <iostream>

using namespace std;

Processor::Processor()
{
	_fileStream << "Queue-A,Queue-B,Queue-E,Processor-Priority-Queue\n";
}

void Processor::Start()
{
	_worker = jthread([this](const stop_token& stopToken) { Work(stopToken); });
}

string Processor::QueueStatus()
{
	stringstream ss;
	ss << _interfaceA.QueueCount() << ',' << _interfaceB.QueueCount() << ',' << _interfaceE.QueueCount() << ',' << QueueCount() << '\n';
	string str = ss.str();
	_fileStream << str << flush;
	return str;
}

size_t Processor::QueueCount() const
{
	const lock_guard lg(_mtxQueue);
	return _messages.size();
}

void Processor::Work(const stop_token& stopToken)
{
	_interfaceA.Start();
	_interfaceB.Start();
	_interfaceE.Start();

	const auto messageCollector = [this](const Message& message) {
		{
			const lock_guard lg(_mtxQueue);
			if (_messages.size() >= 3'000'000)
				return;
			_messages.push(message);
		}
		_nvm.Write(message);
	};

	_interfaceA.Subscribe(messageCollector);
	_interfaceB.Subscribe(messageCollector);
	_interfaceE.Subscribe(messageCollector);

	for (; !stopToken.stop_requested(); this_thread::yield()) {
		unique_lock uniqueLock(_mtxQueue);
		if (_messages.empty())
			continue;

		auto message = _messages.top();
		_messages.pop();
		uniqueLock.unlock();

		switch (message.to) {
		case 'C':
			_interfaceC.Write(message);
			break;
		case 'D':
			_interfaceD.Write(message);
			break;
		[[unlikely]] default:
			cerr << "Unknown destination: " << message.to << std::endl;
			continue;
		}
	}
}
