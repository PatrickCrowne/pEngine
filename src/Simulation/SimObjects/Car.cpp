#include "Car.h"
#include "../Components/MeshRenderer.h"
Car::Car(float offset) {

	carOffset = offset;
	// Create the mesh renderer so the track can be rendered
	MeshRenderer* carMeshRenderer = new MeshRenderer();
	carMeshRenderer->mesh = Mesh::getMesh("models/cube.obj");
	carMeshRenderer->material = Material::getMaterial("materials/m.mat");
	addComponent(carMeshRenderer);

}
