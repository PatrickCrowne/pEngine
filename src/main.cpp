#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm>
#include "Simulator.h"
#include "glstate.hpp"
#include <GL/freeglut.h>
namespace fs = std::filesystem;

// Menu identifiers
const int OBJMENU_TETRAHEDRON = 64;			// Select tetrahedron to view
const int MENU_EXIT = 1;					// Exit application
std::vector<std::string> meshFilenames;		// Paths to .obj files to load

// OpenGL state
std::unique_ptr<GLState> glState;

// Initialization functions
void initGLUT(int* argc, char** argv);
void initMenu();
void findObjFiles();

// Callback functions
void display();
void reshape(GLint width, GLint height);
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
		initMenu();
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
	int width = 800; int height = 600;
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	// Create the window
	glutCreateWindow("FreeGLUT Window");

	// Create a menu

	// GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouseBtn);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
	glutCloseFunc(cleanup);
}

void initMenu() {
	// Create a submenu with all the objects you can view
	findObjFiles();
	int objmenu = glutCreateMenu(menu);
	glutAddMenuEntry("Tetrahedron", OBJMENU_TETRAHEDRON);	// Built-in
	for (int i = 0; i < meshFilenames.size(); i++) {
		glutAddMenuEntry(meshFilenames[i].c_str(), OBJMENU_TETRAHEDRON + i + 1);
	}

	// Create the main menu, adding the objects menu as a submenu
	glutCreateMenu(menu);
	glutAddSubMenu("View object", objmenu);
	glutAddMenuEntry("Exit", MENU_EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void findObjFiles() {
	// Search the models/ directory for any file ending in .obj
	fs::path modelsDir = "models";
	for (auto& di : fs::directory_iterator(modelsDir)) {
		if (di.is_regular_file() && di.path().extension() == ".obj")
			meshFilenames.push_back(di.path().string());
	}
	std::sort(meshFilenames.begin(), meshFilenames.end());
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

// Called when a key is released
void keyRelease(unsigned char key, int x, int y) {
	switch (key) {
	case 27:	// Escape key
		menu(MENU_EXIT);
		break;
	}
}

// Called when a mouse button is pressed or released
void mouseBtn(int button, int state, int x, int y) {
	// Press left mouse button
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		// Start rotating the camera
		glState->beginCameraRotate(glm::vec2(x, y));
	}
	// Release left mouse button
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
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
	if (glState->isCamRotating()) {
		// Rotate the camera if currently rotating
		glState->rotateCamera(glm::vec2(x, y));
		glutPostRedisplay();	// Request redraw
	}
}

// Called when there are no events to process
void idle() {
	// TODO: anything that happens every frame (e.g. movement) should be done here
	// Be sure to call glutPostRedisplay() if the screen needs to update as well
	Simulator::Update();
}

// Called when a menu button is pressed
void menu(int cmd) {
	switch (cmd) {
	// End the program
	case MENU_EXIT:
		glutLeaveMainLoop();
		break;

	// Show the tetrahedron
	case OBJMENU_TETRAHEDRON:
		glState->showTetrahedron();
		glutPostRedisplay();	// Request redraw
		break;

	default:
		// Show the other objects
		if (cmd > OBJMENU_TETRAHEDRON) {
			try {
				glState->showObjFile(meshFilenames[cmd - OBJMENU_TETRAHEDRON - 1]);
				glutPostRedisplay();	// Request redraw
			// Might fail to load object
			} catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
		break;
	}
}

// Called when the window is closed or the event loop is otherwise exited
void cleanup() {
	// Delete the GLState object, calling its destructor,
	// which releases the OpenGL objects
	glState.reset(nullptr);
}
