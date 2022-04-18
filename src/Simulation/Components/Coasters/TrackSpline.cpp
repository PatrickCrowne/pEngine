#include "TrackSpline.h"
#include "../../CSV.h"

#define PI 3.14159265f

TrackSpline::TrackSpline() {

    CSV* csv = new CSV("tracks/test.csv");

    for (int i = 0; i < csv->positions.size(); i++) {
        addNode(csv->positions.at(i));
    }
    
    tieMesh = Mesh::getMesh("models/crossties/arrow_tie.obj", true);

    buildTrackMesh();

}

bool TrackSpline::addNode(glm::vec3 position)
{
    nodes.push_back(position);
    return true;
}

bool TrackSpline::removeNode(int index)
{
    return true;
}

/// <summary>
/// Rebuilds the entire track mesh
/// </summary>
/// <returns></returns>
bool TrackSpline::buildTrackMesh()
{

    if (nodes.size() < 2) return false;

    for (int i = 1; i < nodes.size() - 2; i++) {
        buildTrackMeshSection(i);
    }

}

/// <summary>
/// Clamps value t between m < M
/// </summary>
/// <param name="t"></param>
/// <param name="m"></param>
/// <param name="M"></param>
/// <returns></returns>
float clamp(float t, float m, float M) {
    if (t < m) return m;
    if (t > M) return M;
    return t;
}

TrackMeshSegment* TrackSpline::getTrackMeshSegment(int index) {

    if (trackSegments.count(index) > 0) {
        return trackSegments.at(index);
    }
    return new TrackMeshSegment(index);

}

glm::vec3 TrackSpline::generateRailVertices(glm::vec3 offset, float radius, std::vector<glm::vec3>* vertices, std::vector<int>* triangles, std::vector<glm::vec2>* uvs, float length, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 up) {

    const int railSegmentCount = (int)(length * 10);

    glm::vec3 curPos;
    glm::vec3 lastPos;
    glm::vec3 forward, right;
    glm::vec3 lastUp = glm::vec3(0, 1, 0);

    for (int i = 0; i <= railSegmentCount; i++) {

        curPos = bSpline((float)i / (float)railSegmentCount, p0, p1, p2, p3);
        lastPos = bSpline(((float)i / (float)railSegmentCount) + 0.1f, p0, p1, p2, p3);

        // TODO: Add rolling
        

        forward = glm::normalize(lastPos - curPos);
        right = glm::normalize(glm::cross(forward, up));
        up = -glm::normalize(glm::cross(forward, right));

        

        glm::vec3 position = curPos;
        int vertexCount = vertices->size();

        // Create a ring 
        for (int j = 0; j < SPLINE_RAIL_RESOLUTION; j++) {

            float theta = (((float)j / (float)SPLINE_RAIL_RESOLUTION) * 2.0f * PI) + (offset.z);
            glm::vec3 newPos = glm::vec3(sin(theta) * radius + offset.x, cos(theta) * radius + offset.y, 0);

            // Write vertex data to arrays
            vertices->push_back(position + (newPos.x * right) + (newPos.y * up) + (newPos.z * forward));
            uvs->push_back(glm::vec2((float)j / (float)(SPLINE_RAIL_RESOLUTION + 1), (float)i / (float)railSegmentCount));

            // Add faces
            if (i > 0) {

                if (j < SPLINE_RAIL_RESOLUTION - 1) {
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION + j);
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION + j + 1);
                    triangles->push_back(vertexCount + j + 1);
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION + j);
                    triangles->push_back(vertexCount + j + 1);
                    triangles->push_back(vertexCount + j);
                }
                else {
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION + j);
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION);
                    triangles->push_back(vertexCount);
                    triangles->push_back(vertexCount - SPLINE_RAIL_RESOLUTION + j);
                    triangles->push_back(vertexCount);
                    triangles->push_back(vertexCount + j);
                }

            }

        }

        lastUp = up;

    }

    return lastUp;

}

/// <summary>
/// Generates the start tangent for a bezier spline
/// </summary>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <returns></returns>
glm::vec3 TrackSpline::getStartTangent(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

    glm::vec3 curPos = bSpline(0, p0, p1, p2, p3);
    glm::vec3 lastPos = bSpline(0.001f, p0, p1, p2, p3);

    return glm::normalize(lastPos - curPos);

}

/// <summary>
/// Generates the crossties for a track segment
/// </summary>
/// <param name="vertices"></param>
/// <param name="triangles"></param>
/// <param name="uvs"></param>
/// <param name="length"></param>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <param name="up"></param>
void TrackSpline::generateCrossties(std::vector<glm::vec3>* vertices, std::vector<int>* triangles, std::vector<glm::vec2>* uvs, float length, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 up) {

    int railSegmentCount = (int)(length * 2) - 1;
    if (railSegmentCount < 1) railSegmentCount = 1;

    glm::vec3 curPos;
    glm::vec3 lastPos;
    glm::vec3 forward, right;

    for (int i = 0; i <= railSegmentCount; i++) {

        curPos = bSpline((float)i / (float)railSegmentCount, p0, p1, p2, p3);
        lastPos = bSpline(((float)i / (float)railSegmentCount) + 0.001f, p0, p1, p2, p3);

        // TODO: Add rolling

        forward = glm::normalize(lastPos - curPos);
        right = glm::normalize(glm::cross(forward, up));
        up = -glm::normalize(glm::cross(forward, right));

        glm::vec3 position = curPos;
        int vertexCount = vertices->size();

        for (int j = 0; j < tieMesh->vertices.size(); j++) {

            glm::vec3 vertexPos = tieMesh->vertices.at(j).pos;
            vertices->push_back(position + (vertexPos.x * right) + (vertexPos.y * up) + (vertexPos.z * forward));
            uvs->push_back(tieMesh->vertices.at(j).uv);
            triangles->push_back(vertexCount + j);

        }


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

    TrackMeshSegment* segment = getTrackMeshSegment(index);
    glm::vec3 lastUp = getTrackMeshSegment(index - 1)->endNormal;
    if (index - 1 <= 0) {
        lastUp = glm::vec3(0, 1, 0);
    }


    // Generate baked spline
    bakedSpline.clear();
    // Find the spline segment length

    glm::vec3 p0 = nodes.at(clamp(index - 1, 0, nodes.size() - 1));
    glm::vec3 p1 = nodes.at(clamp(index,     0, nodes.size() - 1));
    glm::vec3 p2 = nodes.at(clamp(index + 1, 0, nodes.size() - 1));
    glm::vec3 p3 = nodes.at(clamp(index + 2, 0, nodes.size() - 1));

    // Move the track segment origin to where the first node is
    glm::vec3 startPos = bSpline(0, p0, p1, p2, p3);
    p0 -= startPos;
    p1 -= startPos;
    p2 -= startPos;
    p3 -= startPos;
    segment->transform.position = startPos;
    

    float length = 0;
    glm::vec3 lastPos = bSpline(0, p0, p1, p2, p3);
    glm::vec3 curPos = lastPos;
    for (float t = 1.0f / (float) SPLINE_SAMPLE_COUNT; t <= 1; t += 1.0f / (float) SPLINE_SAMPLE_COUNT) {
        curPos = bSpline(t, p0, p1, p2, p3);
        length += glm::distance(curPos, lastPos);
        lastPos = curPos;
    }
    // Update the length
    segment->length = length;

    //
    // Resample curve
    //

    // create a step size that will maximize the chance that it will line up nicely at the end
    const float targetStepSize = 0.05f / length;
    float stepSize = 1.0f / length;
    float splinePos = 0;
    float distance = 0;
    lastPos = bSpline(splinePos, p0, p1, p2, p3);
    curPos = lastPos;
    bakedSpline.push_back(lastPos);
    while (splinePos <= 1) {
        
        // Iteratively sample the spline until a step size is found within an acceptable range of accuracy
        for (int iterations = 0; iterations < SPLINE_ITERATION_COUNT; iterations++) {

            curPos = bSpline(splinePos + stepSize, p0, p1, p2, p3);
            distance = glm::distance(curPos, lastPos);
            
            stepSize *= (targetStepSize / distance);

        }
        splinePos += stepSize;
        curPos = bSpline(splinePos, p0, p1, p2, p3);
        bakedSpline.push_back(curPos);
        lastPos = curPos;

    }

    // Sample baked spline to create rails


    // Create vectors to store mesh data
    std::vector<glm::vec3> vertices;
    std::vector<int> triangles;
    std::vector<glm::vec2> uvs;
    
    // Add Rails
    generateRailVertices(glm::vec3(-0.6f, 0, PI / 2.0f), 0.075f, &vertices, &triangles, &uvs, length, p0, p1, p2, p3, lastUp);
    generateRailVertices(glm::vec3(0.6f, 0, -PI / 2.0f), 0.075f, &vertices, &triangles, &uvs, length, p0, p1, p2, p3, lastUp);
    segment->endNormal = generateRailVertices(glm::vec3(0, -0.75f, 0), 0.15f, &vertices, &triangles, &uvs, length, p0, p1, p2, p3, lastUp);

    // Add Crossties
    generateCrossties(&vertices, &triangles, &uvs, length, p0, p1, p2, p3, lastUp);

    glm::vec3 tangent = getStartTangent(p0, p1, p2, p3);

    // Update the mesh
    if (triangles.size() > 0) {
        segment->getMesh()->updateMesh(vertices, triangles, uvs);
    }

    trackSegments.emplace(segment->index, segment);

    return true;

}



/// <summary>
/// Interpolates between the points using the b-Spline algorithm
/// </summary>
/// <param name="t"></param>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <returns></returns>
glm::vec3 TrackSpline::bSpline(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

    float tt = t * t;
    float ttt = t * t * t;

    return
        ((((1.0f - t) * (1.0f - t) * (1.0f - t)) / 6.0f) * p0) +
        (((4.0f - (6.0f * tt) + (3.0f * ttt)) / 6.0f) * p1) +
        (((1.0f + (3.0f * t) + (3.0f * tt) - (3.0f * ttt)) / 6.0f) * p2) +
        ((ttt / 6.0f) * p3);

}