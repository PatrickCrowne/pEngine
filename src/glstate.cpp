#define NOMINMAX
#include "glstate.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util.hpp"
#include "Simulation/Shader.h"
#include "Simulation/Material.h"
#include <iostream>
#include "Simulation/SimObjects/SimObject.h"
#include "Simulation/Scene.h"
#include "Simulation/Simulator.h"
#include "Simulation/AssetReader.h"

// Constructor
GLState::GLState() :
	width(1), height(1),
	fovy(45.0f),
	camCoords(0.0f, 0.0f, 2.0f),
	camRotating(false),
	objMode(OBJMODE_TETRAHEDRON),
	shader(0),
	xformLoc(0),
	vao(0),
	vbuf(0),
	ibuf(0),
	vcount(0) {}

// Destructor
GLState::~GLState() {
	// Release OpenGL resources
	if (shader)	glDeleteProgram(shader);
	if (vao)	glDeleteVertexArrays(1, &vao);
	if (vbuf)	glDeleteBuffers(1, &vbuf);
	if (ibuf)	glDeleteBuffers(1, &ibuf);
}

// Called when OpenGL context is created (some time after construction)
void GLState::initializeGL() {
	// General settings
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// TEMP
	Scene* scene = new Scene();

	SimObject* simObject = new SimObject("simobjects/bunny.simobj");
	scene->registerSimObject(simObject);
	renderers.push_back(simObject->getComponent<MeshRenderer>());

	SimObject* simObject2 = new SimObject("simobjects/testobj.simobj");
	scene->registerSimObject(simObject2);
	renderers.push_back(simObject2->getComponent<MeshRenderer>());

	Simulator::activeScene = scene;

	// END TEMP

	// Initialize OpenGL state
	initShaders();
	initGeometry();
}

/// <summary>
/// Registers a new renderer to the scene, so it can be included in the render loop
/// </summary>
/// <param name="newRenderer"></param>
void GLState::registerRenderer(MeshRenderer* newRenderer) {
	renderers.push_back(newRenderer);
}

// Called when window requests a screen redraw
void GLState::paintGL() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Construct a transformation matrix for the camera
	glm::mat4 xform(1.0f);
	// Perspective projection
	float aspect = (float)width / (float)height;
	glm::mat4 proj = glm::perspective(glm::radians(fovy), aspect, 0.1f, 100.0f);
	// Camera viewpoint
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -camCoords.z));
	view = glm::rotate(view, glm::radians(camCoords.y), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(camCoords.x), glm::vec3(0.0f, 1.0f, 0.0f));
	// Combine transformations
	xform = proj * view;

	for (MeshRenderer* renderer : renderers) {

		renderer->Render(xform);

	}

	/*
	switch (objMode) {
	case OBJMODE_TETRAHEDRON: {
		// Scale down the tetrahedron a bit
		glm::mat4 scale(0.4082f);
		scale[3][3] = 1.0f;
		xform = xform * scale;
		// Send transform matrix to shader
		glUniformMatrix4fv(mat->shader->uniformInputs.at(mat->modelMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(xform));

		// Use our vertex format and buffers
		glBindVertexArray(vao);
		// Draw the geometry
		glDrawElements(GL_TRIANGLES, vcount, GL_UNSIGNED_INT, NULL);
		// Cleanup state
		glBindVertexArray(0);
		break; }

	case OBJMODE_MESH: {
		// Scale and center mesh using its bounding box
		auto meshBB = mesh->boundingBox();
		glm::mat4 fixBB = glm::scale(glm::mat4(1.0f),
			glm::vec3(1.0f / glm::length(meshBB.second - meshBB.first)));
		fixBB = glm::translate(fixBB, -(meshBB.first + meshBB.second) / 2.0f);
		// Concatenate bounding box transform to view transform and upload to shader
		xform = xform * fixBB;
		glUniformMatrix4fv(mat->shader->uniformInputs.at(mat->modelMatrixAttributeName), 1, GL_FALSE, glm::value_ptr(xform));

		// Draw the mesh
		mesh->draw();
		break; }
	}
	*/

	glUseProgram(0);
}

// Called when window is resized
void GLState::resizeGL(int w, int h) {
	// Tell OpenGL the new dimensions of the window
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

// Start rotating the camera (click + drag)
void GLState::beginCameraRotate(glm::vec2 mousePos) {
	camRotating = true;
	initCamRot = glm::vec2(camCoords);
	initMousePos = mousePos;
}

// Stop rotating the camera (mouse button is released)
void GLState::endCameraRotate() {
	camRotating = false;
}

// Use mouse delta to determine new camera rotation
void GLState::rotateCamera(glm::vec2 mousePos) {
	if (camRotating) {
		float rotScale = glm::min(width / 450.0f, height / 270.0f);
		glm::vec2 mouseDelta = mousePos - initMousePos;
		glm::vec2 newAngle = initCamRot + mouseDelta / rotScale;
		newAngle.y = glm::clamp(newAngle.y, -90.0f, 90.0f);
		while (newAngle.x > 180.0f) newAngle.x -= 360.0f;
		while (newAngle.x < -180.0f) newAngle.x += 360.0f;
		if (glm::length(newAngle - glm::vec2(camCoords)) > FLT_EPSILON) {
			camCoords.x = newAngle.x;
			camCoords.y = newAngle.y;
		}
	}
}

// Moves the camera toward / away from the origin (scroll wheel)
void GLState::offsetCamera(float offset) {
	camCoords.z = glm::clamp(camCoords.z + offset, 0.1f, 10.0f);
}

// Display the tetrahedron
void GLState::showTetrahedron() {
	objMode = OBJMODE_TETRAHEDRON;
}

// Display a given .obj file
void GLState::showObjFile(const std::string& filename) {
	// Load the .obj file if it's not already loaded
	if (!mesh || meshFilename != filename) {
		mesh = std::unique_ptr<Mesh>(new Mesh(filename));
		meshFilename = filename;
	}
	objMode = OBJMODE_MESH;
}

// Create shaders and associated state
void GLState::initShaders() {

	//mat = new Material("shaders/m.mat");
	//Shader *testShader = new Shader("shaders/v.glsl", "shaders/f.glsl");

	// Get uniform locations
	//shader = mat->shader->compiledShaderId;
	//xformLoc = mat->shader->uniformInputs.at(mat->modelMatrixAttributeName);
}

// Create geometry buffers and vertex format
void GLState::initGeometry() {
	// Vertices
	std::vector<Vertex> verts {
		  // Position                 // Normal
		{ {  1.0f,  0.0f, -0.7071f }, {  0.8164f,  0.0f,    -0.5773f }, },	// v0
		{ { -1.0f,  0.0f, -0.7071f }, { -0.8164f,  0.0f,    -0.5773f }, },	// v1
		{ {  0.0f,  1.0f,  0.7071f }, {  0.0f,     0.8164f,  0.5773f }, },	// v2
		{ {  0.0f, -1.0f,  0.7071f }, {  0.0f,    -0.8164f,  0.5773f }, },	// v3
	};

	// Triangle indices
	std::vector<GLuint> inds {
		0, 2, 1,	// t0
		1, 2, 3,	// t1
		2, 0, 3,	// t2
		3, 0, 1,	// t3
	};
	vcount = (GLsizei)inds.size();

	// Create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create OpenGL buffers for vertex and index data
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &ibuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);

	// Specify vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));

	// Cleanup state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
