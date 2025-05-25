#pragma once
#include <chrono>
#include <vector>

#include "Err.h"

class Timeline
{
private:
	bool isAssociated_ = false;
	Timeline* associatedTo_ = nullptr;

	uint64_t unitMeasurement_ = 1;
	float_t scale_ = 1;

	std::chrono::time_point<std::chrono::steady_clock> lastStartTime_;
	std::chrono::time_point<std::chrono::steady_clock> lastTime_;
	std::chrono::nanoseconds elapsed_;

	uint64_t lastDelta_ = 0;
	uint64_t deltaBuffer_ = 0;
	uint64_t timeDelta_ = 0;

	bool isStarted_ = false;
	bool isRunning_ = false;

	std::vector<Timeline*> dependents_ = std::vector<Timeline*>();

public:
	explicit Timeline(const uint64_t unitMeasurement = 1, const float_t scale = 1) : unitMeasurement_(unitMeasurement), scale_(scale) { elapsed_ = std::chrono::nanoseconds::zero(); }
	explicit Timeline(Timeline* dependsOn, float_t scale = 1);

	Err Start(bool bypassAssoc = false);
	Err Reset(bool bypassAssoc = false);
	Err Pause(bool bypassAssoc = false);
	Err Resume(bool bypassAssoc = false);

	Err UpdateLastTime();
	Err ChangeUnitMeasurement(uint64_t unitMeasurement, bool bypassAssoc = false);
	Err ChangeScale(float_t scale);

	int64_t GetElapsed() const;
	uint64_t GetDelta() const;
	Err Seek() const;

	bool IsStarted() const;
	bool IsRunning() const;

	Err Associate(Timeline* dependsOn, float_t scale = 1);
	Err Dissociate();
};

namespace timeline
{
	constexpr uint64_t NANOSECOND = 1;
	constexpr uint64_t MICROSECOND = 1000;
	constexpr uint64_t MILLISECOND = 1000000;
	constexpr uint64_t SECOND = 1000000000;
}