#include "Timeline.h"

Timeline::Timeline(Timeline* dependsOn, const float_t scale)
{
	associatedTo_ = dependsOn;
	associatedTo_->dependents_.push_back(this);
	isAssociated_ = true;

	elapsed_ = dependsOn->elapsed_;
	isRunning_ = dependsOn->isRunning_;
	isStarted_ = dependsOn->isStarted_;
	unitMeasurement_ = dependsOn->unitMeasurement_;
	scale_ = scale;
}

Err Timeline::Start(const bool bypassAssoc)
{
	if (isStarted_)
		return error_const::TIMELINE_ALREADY_STARTED;

	if (isAssociated_ && !bypassAssoc)
		return error_const::TIMELINE_IS_ASSOCIATED;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	isStarted_ = true;
	isRunning_ = true;

	// Start dependents
	for (Timeline* t : dependents_)
	{
		Err err = t->Start(true);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err Timeline::Reset(const bool bypassAssoc)
{
	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	if (isAssociated_ && !bypassAssoc)
		return error_const::TIMELINE_IS_ASSOCIATED;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	elapsed_ = std::chrono::nanoseconds::zero();
	timeDelta_ = 0;

	// Reset dependents
	for (Timeline* t : dependents_)
	{
		Err err = t->Reset(true);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err Timeline::Pause(const bool bypassAssoc)
{
	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	if (!isRunning_)
		return error_const::TIMELINE_IS_PAUSED;

	if (isAssociated_ && !bypassAssoc)
		return error_const::TIMELINE_IS_ASSOCIATED;

	elapsed_ += std::chrono::steady_clock::now() - lastStartTime_;
	deltaBuffer_ = (std::chrono::steady_clock::now() - lastTime_).count();
	lastDelta_ = timeDelta_;
	isRunning_ = false;

	// Pause dependents
	for (Timeline* t : dependents_)
	{
		Err err = t->Pause(true);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err Timeline::Resume(const bool bypassAssoc)
{
	if (isRunning_)
		return error_const::TIMELINE_IS_RUNNING;

	if (isAssociated_ && !bypassAssoc)
		return error_const::TIMELINE_IS_ASSOCIATED;

	lastStartTime_ = std::chrono::steady_clock::now();
	lastTime_ = std::chrono::steady_clock::now();
	isRunning_ = true;

	// Resume dependents
	for (Timeline* t : dependents_)
	{
		Err err = t->Resume(true);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err Timeline::ChangeUnitMeasurement(const uint64_t unitMeasurement, bool bypassAssoc)
{
	if (isAssociated_ && !bypassAssoc)
		return error_const::TIMELINE_IS_ASSOCIATED;

	unitMeasurement_ = unitMeasurement;

	// Change dependents' unitMeasurement
	for (Timeline* t : dependents_)
	{
		Err err = t->ChangeUnitMeasurement(unitMeasurement, true);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err Timeline::ChangeScale(const float_t scale)
{
	scale_ = scale;
	return error_const::SUCCESS;
}

int64_t Timeline::GetElapsed() const
{
	if (!isStarted_)
		return 0;

	if (!isRunning_)
		return elapsed_.count() / static_cast<int64_t>(static_cast<float_t>(unitMeasurement_) * scale_);

	return (std::chrono::steady_clock::now() - lastStartTime_ + elapsed_).count() / static_cast<int64_t>(static_cast<float_t>(unitMeasurement_) * scale_);
}

Err Timeline::UpdateLastTime()
{
	if (!isRunning_)
		return error_const::SUCCESS;

	if (!isStarted_)
		return error_const::TIMELINE_IS_STOPPED;

	if (isAssociated_)
		return error_const::TIMELINE_IS_ASSOCIATED;

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

Err Timeline::Associate(Timeline* dependsOn, const float_t scale)
{
	if (isAssociated_)
		return error_const::TIMELINE_IS_ASSOCIATED;

	associatedTo_ = dependsOn;
	associatedTo_->dependents_.push_back(this);
	isAssociated_ = true;

	elapsed_ = dependsOn->elapsed_;
	isRunning_ = dependsOn->isRunning_;
	isStarted_ = dependsOn->isStarted_;
	unitMeasurement_ = dependsOn->unitMeasurement_;
	scale_ = scale;

	return error_const::SUCCESS;
}

Err Timeline::Dissociate()
{
	if (!isAssociated_)
		return error_const::TIMELINE_IS_NOT_ASSOCIATED;

	// Remove from association vector
	std::vector<Timeline*>* dependentsVector = &associatedTo_->dependents_;
	dependentsVector->erase(std::remove(dependentsVector->begin(), dependentsVector->end(), this), dependentsVector->end());

	associatedTo_ = nullptr;
	isAssociated_ = false;

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
