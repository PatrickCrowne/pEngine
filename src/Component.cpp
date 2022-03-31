#include "Component.h"
#include "Simulator.h"
#include <iostream>

/// <summary>
/// Called at the start of this Component's life
/// </summary>
void Component::Awake() {

}

/// <summary>
/// Called every frame this component is awake
/// </summary>
void Component::Update() {

	float time = Simulator::deltaTime;
	std::cout << "Frame time: " << time << "ms" << "\n";

}

/// <summary>
/// Called every fixed time interval set by the simulator
/// </summary>
void Component::FixedUpdate() {

}

/// <summary>
/// Called when this component is destroyed
/// </summary>
void Component::Destroy() {

}