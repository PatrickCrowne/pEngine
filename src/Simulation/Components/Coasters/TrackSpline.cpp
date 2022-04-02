#include "TrackSpline.h"

bool TrackSpline::addNode(glm::vec3 position)
{
    return false;
}

bool TrackSpline::removeNode(int index)
{
    return false;
}

/// <summary>
/// Rebuilds the entire track mesh
/// </summary>
/// <returns></returns>
bool TrackSpline::buildTrackMesh()
{

    if (nodes.size() < 2) return false;

    for (int i = 0; i < nodes.size(); i++) {
        buildTrackMeshSection(i);
    }

}

/// <summary>
/// Builds a 
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
bool TrackSpline::buildTrackMeshSection(int index) {

    if (index < 0) return false;
    if (index > nodes.size() - 1) return false;

    // TODO: Generate baked spline

    // TODO: Sample baked spline to create rails

    // TODO: Add crossties to rails to complete the track model

}
