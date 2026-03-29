#pragma once

#include <chrono>

// 1Kb structure
struct Message final {
	std::chrono::system_clock::time_point dateTime;
	char from;
	char to;
	uint8_t message[118];

	struct Compare final {
		bool operator()(const Message& a, const Message& b) const
		{
			{
				const auto now = std::chrono::system_clock::now();

				const int prio_a = a.from + (now - a.dateTime > std::chrono::seconds(10) ? 10 : 0);
				const int prio_b = b.from + (now - b.dateTime > std::chrono::seconds(10) ? 10 : 0);

				if (prio_a != prio_b)
					return prio_a < prio_b;
			}

			return a.dateTime > b.dateTime;
		}
	};
};
