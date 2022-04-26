#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm>
#include "Simulation/Simulator.h"
#include "glstate.hpp"
#include <GL/freeglut.h>

namespace fs = std::filesystem;

// Menu identifiers
const int MENU_EXIT = 1;					// Exit application

// OpenGL state
std::unique_ptr<GLState> glState;

// Pressed key list
bool pressedKeys[512];

// Initialization functions
void initGLUT(int* argc, char** argv);
void initMenu();

// Callback functions
void display();
void reshape(GLint width, GLint height);
void keyPress(unsigned char key, int x, int y);
void keyRelease(unsigned char key, int x, int y);
void mouseBtn(int button, int state, int x, int y);
void mouseMove(int x, int y);
void idle();
void menu(int cmd);
void cleanup();

// Program entry point
int main(int argc, char** argv) {
	try {
		// Create the window and menu
		initGLUT(&argc, argv);
		//initMenu();
		// Initialize OpenGL (buffers, shaders, etc.)
		glState = std::unique_ptr<GLState>(new GLState());
		glState->initializeGL();

	} catch (const std::exception& e) {
		// Handle any errors
		std::cerr << "Fatal error: " << e.what() << std::endl;
		cleanup();
		return -1;
	}

	// Execute main loop
	glutMainLoop();

	return 0;
}

// Setup window and callbacks
void initGLUT(int* argc, char** argv) {
	// Set window and context settings
	int width = 1280; int height = 720;
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	// Create the window
	int mainWindow = glutCreateWindow("pEngine");

	

	// GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouseBtn);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutIdleFunc(idle);
	glutCloseFunc(cleanup);
}

void initMenu() {

	// Create the main menu, adding the objects menu as a submenu
	glutCreateMenu(menu);
	glutAddMenuEntry("Exit", MENU_EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

// Called whenever a screen redraw is requested
void display() {
	// Tell the GLState to render the scene
	glState->paintGL();

	// Scene is rendered to the back buffer, so swap the buffers to display it
	glutSwapBuffers();
}

// Called when the window is resized
void reshape(GLint width, GLint height) {
	// Tell OpenGL the new window size
	glState->resizeGL(width, height);
}

// Called when a key is pressed
void keyPress(unsigned char key, int x, int y) {
	pressedKeys[key] = true;
}

// Called when a key is released
void keyRelease(unsigned char key, int x, int y) {
	pressedKeys[key] = false;
	switch (key) {
	case 27:	// Escape key
		menu(MENU_EXIT);
		break;
	}
}

// Called when a mouse button is pressed or released
void mouseBtn(int button, int state, int x, int y) {
	// Press left mouse button
	if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON && !glState->isCamRotating()) {
		// Start rotating the camera
		glState->beginCameraRotate(glm::vec2(x, y));
	}
	// Release left mouse button
	else if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON && glState->isCamRotating()) {
		// Stop camera rotation
		glState->endCameraRotate();
	}
	// Scroll wheel up
	if (button == 3) {
		// "Zoom in" (actually just moves the camera)
		glState->offsetCamera(-0.1f);
		glutPostRedisplay();	// Request redraw
	}
	// Scroll wheel down
	if (button == 4) {
		// "Zoom out"
		glState->offsetCamera(0.1f);
		glutPostRedisplay();	// Request redraw
	}
}

// Called when the mouse moves
void mouseMove(int x, int y) {
	//if (glState->isCamRotating()) {
		// Rotate the camera if currently rotating
		glState->rotateCamera(glm::vec2(x, y));
	//}
}

#define CAM_SPEED 60

// Called when there are no events to process (i.e. every frame)
void idle() {

	glm::vec3 moveOffset = glm::vec3(0, 0, 0);

	if (pressedKeys['w']) { // W Key
		moveOffset += glm::vec3(0, 0, Simulator::deltaTime * CAM_SPEED);
	}
	if (pressedKeys['s']) { // S Key
		moveOffset += glm::vec3(0, 0, -Simulator::deltaTime * CAM_SPEED);
	}
	if (pressedKeys['a']) { // A Key
		moveOffset += glm::vec3(Simulator::deltaTime * CAM_SPEED, 0, 0);
	}
	if (pressedKeys['d']) { // D Key
		moveOffset += glm::vec3(-Simulator::deltaTime * CAM_SPEED, 0, 0);
	}

	glState->moveCamera(moveOffset);

	Simulator::Update();
	glutPostRedisplay();

}

// Called when a menu button is pressed
void menu(int cmd) {
	switch (cmd) {
	// End the program
	case MENU_EXIT:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

// Called when the window is closed or the event loop is otherwise exited
void cleanup() {
	// Delete the GLState object, calling its destructor,
	// which releases the OpenGL objects
	glState.reset(nullptr);
}
