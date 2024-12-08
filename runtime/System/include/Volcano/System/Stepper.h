//
//
#ifndef VOLCANO_SYSTEM_STEPPER_H
#define VOLCANO_SYSTEM_STEPPER_H

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Stepper {
public:
	Stepper(uint32_t max = 60)
		: last_(Clock::now())
		, count_last_(last_)
		, elapsed_min_(Precision(Precision::period::den / std::clamp(max, 1u, 1000u)))
		, count_(0)
		, count_per_second_(0) {
	}

	Stepper(const Stepper&) = default;
	Stepper& operator=(const Stepper&) = default;

public:
	Duration elapsed() const {
		return elapsed_;
	}

	uint32_t countPerSecond() const {
		return count_per_second_;
	}

	uint32_t max() const {
		auto tmp = std::chrono::duration_cast<Precision>(elapsed_min_);
		return (Precision::period::den / tmp.count());
	}

	void setMax(uint32_t v) {
		auto tmp = std::clamp(v, 1u, 1000u);
		elapsed_min_ = Precision(Precision::period::den / tmp);
	}

	void reset() {
		last_ = Clock::now();
		count_last_ = last_;
		count_ = 0;
		count_per_second_ = 0;
	}

	bool step(Duration slice = 5ms) {
		auto current = Clock::now();
		auto elapsed = current - last_;
		if (elapsed < elapsed_min_) {
			auto left = elapsed_min_ - elapsed;
			SDL_Delay(std::chrono::duration_cast<std::chrono::milliseconds>(left).count());
			//std::this_thread::sleep_for(left < slice ? left : slice);
			return false;
		}

		elapsed_ = elapsed;
		last_ = current;
		count_ += 1;

		if ((current - count_last_) >= 1s) {
			count_per_second_ = count_;
			count_ = 0;
			count_last_ = current;
		}

		return true;
	}

private:
	using Precision = std::chrono::milliseconds;

	TimePoint last_;
	TimePoint count_last_;
	Duration elapsed_min_;
	Duration elapsed_;
	uint32_t count_;
	uint32_t count_per_second_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_STEPPER_H
