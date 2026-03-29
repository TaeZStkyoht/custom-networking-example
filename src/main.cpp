#include "Processor.hpp"

#include <iostream>

#include <csignal>

using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time({})));

	static atomic_bool run = true;

	const auto signalHandler = [](int) {
		puts("signal caught!");
		run = false;
	};
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	Processor processor;
	processor.Start();

	for (;; this_thread::sleep_for(1s)) {
		if (!run) [[unlikely]]
			break;

		cout << processor.QueueStatus();
	}

	return EXIT_SUCCESS;
}
