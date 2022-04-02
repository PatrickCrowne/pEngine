#include "TrackMeshSegment.h"
#include "../Components/MeshRenderer.h"

/// <summary>
/// Initializes this track mesh segment
/// </summary>
TrackMeshSegment::TrackMeshSegment()
{
	// Create the mesh renderer so the track can be rendered
	trackMeshRenderer = new MeshRenderer();
	addComponent(trackMeshRenderer);

}

/// <summary>
/// Returns the reference to the mesh used for rendering
/// </summary>
/// <returns></returns>
Mesh* TrackMeshSegment::getMesh()
{
	return trackMeshRenderer->mesh;
}
