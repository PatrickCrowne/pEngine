#ifndef GLSTATE_HPP
#define GLSTATE_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"
#include "Simulation/mesh.hpp"
#include "Simulation/Material.h"
#include "Simulation/Components/MeshRenderer.h"

// Manages OpenGL state, e.g. camera transform, objects, shaders
class GLState {
public:
	GLState();
	~GLState();
	// Disallow copy, move, & assignment
	GLState(const GLState& other) = delete;
	GLState& operator=(const GLState& other) = delete;
	GLState(GLState&& other) = delete;
	GLState& operator=(GLState&& other) = delete;

	// Callbacks
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	// Camera control
	bool isCamRotating() const { return camRotating; }
	void beginCameraRotate(glm::vec2 mousePos);
	void endCameraRotate();
	void rotateCamera(glm::vec2 mousePos);
	void offsetCamera(float offset);
	void moveCamera(glm::vec3 offset);

	// Object Registration
	static void registerRenderer(MeshRenderer*);
	static void unregisterRenderer(MeshRenderer*);

	// Per-vertex attributes
	struct Vertex {
		glm::vec3 pos;		// Position
		glm::vec3 norm;		// Normal
	};

	

protected:

	// Camera state
	int width, height;		// Width and height of the window
	float fovy;				// Vertical field of view in degrees
	glm::vec3 camCoords;	// Camera spherical coordinates
	glm::vec3 camPos;		// Camera euclidean coordinates
	bool camRotating;		// Whether camera is currently rotating
	glm::vec2 initCamRot;	// Initial camera rotation on click
	glm::vec2 initMousePos;	// Initial mouse position on click

	static std::vector<MeshRenderer*> renderers; // List of renderers for this scene

};

#endif
