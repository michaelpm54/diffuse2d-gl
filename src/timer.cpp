#include "timer.h"

using namespace std::chrono;

Timer::Timer()
{
	_started = steady_clock::now();
}

int Timer::msElapsedSinceConstruction()
{
	return duration_cast<milliseconds>((steady_clock::now() - _started)).count();
}

int Timer::msElapsedSinceReset()
{
	return duration_cast<milliseconds>((steady_clock::now() - _saved)).count();
}

void Timer::Reset()
{
	_saved = steady_clock::now();
}
