#pragma once
#include <chrono>

#define TimePoint std::chrono::steady_clock::time_point
#define NANO_IN_SECOND 1000000000.0f

namespace Simulator {

	extern float deltaTime;
	extern int framesPerSecond;
	void CalculateFrameTime();
	void Update();

}

