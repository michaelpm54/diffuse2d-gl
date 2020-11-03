#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer();
	~Timer() = default;

	int msElapsedSinceConstruction();
	int msElapsedSinceReset();
	void Reset();

private:
	std::chrono::time_point<std::chrono::steady_clock> _started;
	std::chrono::time_point<std::chrono::steady_clock> _saved;
};

#endif // TIMER_H
