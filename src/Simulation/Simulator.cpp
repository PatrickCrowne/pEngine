#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Simulator.h"

using namespace Simulator;

float Simulator::deltaTime;
int Simulator::framesPerSecond;

TimePoint frameStartTime;
TimePoint lastFrameStartTime;

/// <summary>
/// Calculates the time since the last frame
/// </summary>
void Simulator::CalculateFrameTime() {
	frameStartTime = std::chrono::high_resolution_clock::now();
	Simulator::deltaTime = (frameStartTime - lastFrameStartTime).count() / NANO_IN_SECOND;
	lastFrameStartTime = frameStartTime;
	Simulator::framesPerSecond = 1.0f / Simulator::deltaTime;
}

/// <summary>
/// Updates the simulation each frame
/// </summary>
/// <returns></returns>
void Simulator::Update() {
	// Calculate frame timing
	CalculateFrameTime();
}