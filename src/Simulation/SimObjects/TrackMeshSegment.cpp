#include "TrackMeshSegment.h"
#include "../Components/MeshRenderer.h"

/// <summary>
/// Initializes this track mesh segment
/// </summary>
TrackMeshSegment::TrackMeshSegment(int i)
{

	index = i;
	length = 0;
	// Create the mesh renderer so the track can be rendered
	trackMeshRenderer = new MeshRenderer();
	trackMeshRenderer->mesh = Mesh::getMesh("models/cube.obj");
	trackMeshRenderer->material = Material::getMaterial("materials/m.mat");
	addComponent(this->trackMeshRenderer);

	transform.SetEuler(0, 90, 0);

}

/// <summary>
/// Returns the reference to the mesh used for rendering
/// </summary>
/// <returns></returns>
Mesh* TrackMeshSegment::getMesh()
{
	return trackMeshRenderer->mesh;
}
