#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "../../SimObjects/SimObject.h"
#include "../../SimObjects/TrackMeshSegment.h"
#include "../../SimObjects/CoasterStyle.h"

#define SPLINE_STEP_SIZE 0.05f
#define SPLINE_SAMPLE_COUNT 100
#define SPLINE_ITERATION_COUNT 5
#define SPLINE_RAIL_RESOLUTION 12

class TrackSpline
{

public:
	TrackSpline() {}
	TrackSpline(std::string, CoasterStyle*);
	~TrackSpline() {}
	bool addNode(glm::vec3, glm::vec3);
	bool removeNode(int);
	bool buildTrackMesh();
	glm::vec3 getPosition(float t);
	glm::quat getRotation(float t);
	glm::vec3 getSplinePosition(float t);
	glm::vec3 getSplineNormal(float t);
	float length();


private:
	glm::vec3 getStartTangent(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 generateRailVertices(glm::vec3 offset, float radius, std::vector<glm::vec3>* vertices, std::vector<int>* triangles, std::vector<glm::vec2>* uvs, float length, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	void generateCrossties(std::vector<glm::vec3>* vertices, std::vector<int>* triangles, std::vector<glm::vec2>* uvs, float length, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	bool buildTrackMeshSection(int index);
	TrackMeshSegment *getTrackMeshSegment(int);
	glm::vec3 bSpline(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 cubicSpline(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void GenerateBakedSpline();
	std::vector<glm::vec3> nodes;
	std::vector<glm::vec3> nodeNormals;
	std::map<int, TrackMeshSegment*> trackSegments;
	std::vector<glm::vec3> bakedSpline;
	std::vector<glm::vec3> bakedNormal;
	Mesh* tieMesh;
	CoasterStyle* coasterStyle;

};

