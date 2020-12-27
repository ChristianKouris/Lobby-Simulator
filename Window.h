#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Light.h"
#include "Lobby.h"
#include "AstroStill.h"
#include "Bound.h"
#include "ParticleSystem.h"

#include <vector>

class Window
{
private:
	static double timeToSpawn;
	static double timeToDespawn;
	static bool isHeld;
	static bool isMove;
	static double lastPoint;
	static glm::vec3 direction;
	static bool isSpawnPart;
	static bool isDespawnPart;

public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	
	// Objects to Render
	static vector<glm::vec3> available_colors;
	static Bound*  boundaries;
	static Light* light;
	static Lobby* lobby;
	static AstroStill* player;
	static vector<AstroStill*> astronauts;
	static ParticleSystem* spawnParticles;
	static ParticleSystem* despawnParticles;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
	static GLuint astroShaders;
	static GLuint particleShaders;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void spawnAstro();
	static void despawnAstro();
	static glm::vec3 getColor();
	static glm::vec3 trackBallMapping(double x, double y);
	
};

#endif
