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
#include "Simulation/Components/Coasters/TrackSpline.h"

std::vector<MeshRenderer*> GLState::renderers;
std::vector<MeshRenderer*> GLState::uiRenderers;
glm::vec3 GLState::camPos;

// Constructor
GLState::GLState() :
	width(1), height(1),
	fovy(45.0f),
	camCoords(0.0f, 0.0f, 2.0f),
	camRotating(false),
	initCamRot(glm::vec2(0, 0)),
	initMousePos(glm::vec2(0, 0))
	{}

// Destructor
GLState::~GLState() {
	// Release OpenGL resources
}

// Called when OpenGL context is created (some time after construction)
void GLState::initializeGL() {

	// General settings
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	// TEMP
	Scene* scene = new Scene();

	SimObject* simObject = new SimObject("simobjects/bunny.simobj");
	scene->registerSimObject(simObject);

	SimObject* simObject2 = new SimObject("simobjects/test.simobj");
	scene->registerSimObject(simObject2);

	SimObject* simObject3 = new SimObject("simobjects/skybox.simobj");
	scene->registerSimObject(simObject3);

	SimObject* simObject4 = new SimObject("simobjects/groundplane.simobj");
	scene->registerSimObject(simObject4);

	TrackSpline* trackSpline = new TrackSpline();

	Simulator::activeScene = scene;

	// END TEMP

	// Build GUI
	

}

/// <summary>
/// Registers a new renderer to the scene, so it can be included in the render loop
/// </summary>
/// <param name="newRenderer"></param>
void GLState::registerRenderer(MeshRenderer* newRenderer) {
	renderers.push_back(newRenderer);
}

/// <summary>
/// Unregisters a renderer from the GLState
/// </summary>
/// <param name="renderer"></param>
void GLState::unregisterRenderer(MeshRenderer* renderer) {
	renderers.erase(std::find(renderers.begin(), renderers.end(), renderer));
}

/// <summary>
/// Registers a new UI renderer to the scene, so it can be included in the render loop
/// </summary>
/// <param name="newRenderer"></param>
void GLState::registerUIRenderer(MeshRenderer* newRenderer) {
	uiRenderers.push_back(newRenderer);
}

/// <summary>
/// Unregisters a UI renderer from the GLState
/// </summary>
/// <param name="renderer"></param>
void GLState::unregisterUIRenderer(MeshRenderer* renderer) {
	uiRenderers.erase(std::find(uiRenderers.begin(), uiRenderers.end(), renderer));
}

// Called when window requests a screen redraw
void GLState::paintGL() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Construct a transformation matrix for the camera
	glm::mat4 xform(1.0f);
	// Perspective projection
	float aspect = (float)width / (float)height;
	glm::mat4 proj = glm::perspective(glm::radians(fovy), aspect, 0.1f, 10000.0f);
	// Camera viewpoint
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::rotate(view, glm::radians(camCoords.y), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(camCoords.x), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, camPos);
	// Combine transformations
	xform = proj * view;

	// Render all renderers in the scene with the given view projection matrix
	for (MeshRenderer* renderer : renderers) {
		renderer->Render(xform);
	}

	// Render GUI
	// Construct an orthographic matrix for the camera
	// Perspective projection
	aspect = (float)width / (float)height;
	proj = glm::ortho(1.0f, aspect, 0.1f, 100.0f);
	xform = proj;

	for (MeshRenderer* renderer : uiRenderers) {
		renderer->Render(xform);
	}
	

	// Clear the shader reference
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

void GLState::moveCamera(glm::vec3 offset) {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::rotate(view, glm::radians(-camCoords.x), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(-camCoords.y), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 v(view * glm::vec4(offset.x, offset.y, offset.z, 0));

	camPos += v;

}

// Moves the camera toward / away from the origin (scroll wheel)
void GLState::offsetCamera(float offset) {
	camCoords.z = glm::clamp(camCoords.z + offset, 0.1f, 100.0f);
}
