#pragma once
#include <chrono>
#include "Err.h"

class Timeline
{
private:
	bool isAssociated_ = false;
	const Timeline* associatedTo_ = nullptr;

	uint32_t unitMeasurement_ = 1;

	std::chrono::time_point<std::chrono::steady_clock> lastStartTime_;
	std::chrono::time_point<std::chrono::steady_clock> lastTime_;
	std::chrono::nanoseconds elapsed_;

	uint64_t lastDelta_ = 0;
	uint64_t deltaBuffer_ = 0;
	uint64_t timeDelta_ = 0;

	bool isStarted_ = false;
	bool isRunning_ = false;

public:
	explicit Timeline(const uint32_t unitMeasurement = 1) : unitMeasurement_(unitMeasurement) { elapsed_ = std::chrono::nanoseconds::zero(); }
	explicit Timeline(const Timeline* t, uint32_t scale = 1);

	Err Start();
	Err Reset();
	Err Pause();
	Err Resume();

	Err UpdateLastTime();
	Err ChangeUnitMeasurement(uint32_t unitMeasurement);

	uint64_t GetElapsed() const;
	uint64_t GetDelta() const;
	Err Seek() const;

	bool IsStarted() const;
	bool IsRunning() const;

	Err Associate(const Timeline& t);
	Err Dissociate();
};

namespace timeline
{
	constexpr uint32_t NANOSECOND = 1;
	constexpr uint32_t MICROSECOND = 1000;
	constexpr uint32_t MILLISECOND = 1000000;
	constexpr uint32_t SECOND = 1000000000;
}