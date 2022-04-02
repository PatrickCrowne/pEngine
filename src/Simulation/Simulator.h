#pragma once
#include <chrono>
#include "Scene.h"

#define TimePoint std::chrono::steady_clock::time_point
#define NANO_IN_SECOND 1000000000.0f

namespace Simulator {

	extern double deltaTime;
	extern double time;
	extern int framesPerSecond;
	extern Scene *activeScene;
	void CalculateFrameTime();
	void Update();

}

