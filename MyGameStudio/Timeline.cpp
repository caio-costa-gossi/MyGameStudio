#include "Timeline.h"

Timeline::Timeline(const Timeline* t, uint32_t scale)
{
	associatedTo_ = t;
	isAssociated_ = true;

	elapsed_ = t->elapsed_;
	isRunning_ = t->isRunning_;
	isStarted_ = t->isStarted_;
}

Err Timeline::Start()
{
	if (isStarted_)
		return error_const::TIMELINE_ALREADY_STARTED;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	isStarted_ = true;
	isRunning_ = true;

	return error_const::SUCCESS;
}

Err Timeline::Reset()
{
	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	elapsed_ = std::chrono::nanoseconds::zero();
	timeDelta_ = 0;

	return error_const::SUCCESS;
}

Err Timeline::Pause()
{
	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	if (!isRunning_)
		return error_const::TIMELINE_IS_PAUSED;

	elapsed_ += std::chrono::steady_clock::now() - lastStartTime_;
	deltaBuffer_ = (std::chrono::steady_clock::now() - lastTime_).count();
	lastDelta_ = timeDelta_;

	isRunning_ = false;

	return error_const::SUCCESS;
}

Err Timeline::Resume()
{
	if (isRunning_)
		return error_const::TIMELINE_IS_RUNNING;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	isRunning_ = true;

	return error_const::SUCCESS;
}

Err Timeline::ChangeUnitMeasurement(const uint32_t unitMeasurement)
{
	unitMeasurement_ = unitMeasurement;
	return error_const::SUCCESS;
}

uint64_t Timeline::GetElapsed() const
{
	if (!isStarted_)
		return 0;

	if (!isRunning_)
		return elapsed_.count() / unitMeasurement_;

	return (std::chrono::steady_clock::now() - lastStartTime_ + elapsed_).count() / unitMeasurement_;
}

Err Timeline::UpdateLastTime()
{
	if (!isRunning_)
		return error_const::SUCCESS;

	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	timeDelta_ = (std::chrono::steady_clock::now() - lastTime_).count() + deltaBuffer_;
	lastTime_ = std::chrono::steady_clock::now();

	deltaBuffer_ = 0;

	return error_const::SUCCESS;
}

uint64_t Timeline::GetDelta() const
{
	return timeDelta_ / unitMeasurement_;
}

Err Timeline::Seek() const
{
	return error_const::SUCCESS;
}

Err Timeline::Associate(const Timeline& t)
{
	if (isAssociated_)
		return error_const::TIMELINE_IS_ASSOCIATED;

	return error_const::SUCCESS;
}

Err Timeline::Dissociate()
{
	if (!isAssociated_)
		return error_const::TIMELINE_IS_NOT_ASSOCIATED;

	return error_const::SUCCESS;
}

bool Timeline::IsRunning() const
{
	return isRunning_;
}

bool Timeline::IsStarted() const
{
	return isStarted_;
}
