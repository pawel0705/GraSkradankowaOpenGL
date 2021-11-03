#include "pch.h"
#include "timer.h"

void Timer::startTimer(const std::string& name)
{
	start[name] = std::chrono::steady_clock::now();
}

void Timer::stopTimer(const std::string& name)
{
	stop[name] = std::chrono::steady_clock::now();
}

double Timer::getCurrentDurationInSeconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start.at(name)).count() / 1000000000.0;
}

double Timer::getCurrentDurationInMiliseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start.at(name)).count() / 1000000.0;
}

double Timer::getCurrentDurationInMicroseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start.at(name)).count() / 1000.0;
}

double Timer::getCurrentDurationInNanoseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start.at(name)).count();
}


double Timer::getMeasuredDurationInSeconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(stop.at(name) - start.at(name)).count() / 1000000000.0;
}

double Timer::getMeasuredDurationInMiliseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(stop.at(name) - start.at(name)).count() / 1000000.0;
}

double Timer::getMeasuredDurationInMicroseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(stop.at(name) - start.at(name)).count() / 1000.0;
}

double Timer::getMeasuredDurationInNanoseconds(const std::string& name) const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(stop.at(name) - start.at(name)).count();
}
