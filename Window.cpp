#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Light*      Window::light;
Lobby*		Window::lobby;
AstroStill*	Window::player;
Bound*		Window::boundaries; 
ParticleSystem* Window::spawnParticles;
ParticleSystem* Window::despawnParticles;
vector<glm::vec3> Window::available_colors;
vector<AstroStill*> Window::astronauts;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 20, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram; 
GLuint Window::astroShaders;
GLuint Window::particleShaders;

double Window::timeToSpawn;
double Window::timeToDespawn;
bool Window::isHeld = false;
bool Window::isMove = false;
bool Window::isSpawnPart = false;
bool Window::isDespawnPart = false;
double Window::lastPoint;
glm::vec3 Window::direction(0, 0, 0);



bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/lobbyshader.vert", "shaders/lobbyshader.frag");
	astroShaders = LoadShaders("shaders/astroshader.vert", "shaders/astroshader.frag");
	particleShaders = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{

	light = new Light(glm::vec3(0, 1, 1));
	lobby = new Lobby();
	boundaries = new Bound();
	//initialize the boundaries for the walls and boxes
	boundaries->addPlane(8, glm::vec3(0, 0, 1));
	boundaries->addPlane(8.5, glm::vec3(0, 0, -1));
	boundaries->addPlane(16, glm::vec3(1, 0, 0));
	boundaries->addPlane(17, glm::vec3(-1, 0, 0));
	boundaries->addPlane(14, glm::normalize(glm::vec3(1, 0, -1)));
	boundaries->addPlane(14, glm::normalize(glm::vec3(-1, 0, -1)));
	boundaries->addSphere(glm::vec3(-9, 0, -0.6), 2.6);
	boundaries->addSphere(glm::vec3(11, 0, -4), 2.6);
	srand(time(NULL));
	// initialize the 12 colors
	available_colors.push_back(glm::vec3(0.24, 0.28, 0.31)); available_colors.push_back(glm::vec3(0.07, 0.18, 0.82)); 
	available_colors.push_back(glm::vec3(0.44, 0.29, 0.11)); available_colors.push_back(glm::vec3(0.22, 1, 0.87));
	available_colors.push_back(glm::vec3(0.07, 0.5, 0.17)); available_colors.push_back(glm::vec3(0.31, 0.94, 0.22));
	available_colors.push_back(glm::vec3(0.95, 0.49, 0.05)); available_colors.push_back(glm::vec3(0.93, 0.33, 0.73)); 
	available_colors.push_back(glm::vec3(0.42, 0.18, 0.74)); available_colors.push_back(glm::vec3(0.77, 0.07, 0.07));
	available_colors.push_back(glm::vec3(0.84, 0.87, 0.95));available_colors.push_back(glm::vec3(0.96, 0.96, 0.34));
	player = new AstroStill(getColor());
	astronauts.push_back(player);
	timeToSpawn = glfwGetTime() + 5.0 + (double)(rand()%1000) / 100;
	timeToDespawn = glfwGetTime() + 15.0 + (double)(rand() % 1000) / 100;
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete light;
	delete lobby;
	delete boundaries;
	for (AstroStill* astro : astronauts) {
		delete astro;
	}

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
	glDeleteProgram(astroShaders);

}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here
	glm::vec3 collisionNorm = boundaries->environmentCollide(player->getCenter(), player->getRadius());
	if (glm::length(collisionNorm) > 0.01) {
		player->reflect(collisionNorm);
	}
	else {
		for (int i = 1; i < astronauts.size(); i++) {
			collisionNorm = boundaries->spheresCollide(player->getCenter(), player->getRadius(), astronauts[i]->getCenter(), astronauts[i]->getRadius());
			if (glm::length(collisionNorm) > 0.01) {
				player->reflect(collisionNorm);
				break;
			}
		}
	}
	if (glm::length(collisionNorm) < 0.01 && isMove) player->move(direction);
	//moving the AI
	for (int i = 1; i < astronauts.size(); i++) {
		//check to see if player collides with boundaries
		glm::vec3 collisionNorm = boundaries->environmentCollide(astronauts[i]->getCenter(), astronauts[i]->getRadius());
		if (glm::length(collisionNorm) > 0.01) {
			astronauts[i]->reflect(collisionNorm);
			continue;
		}
		//check to see if player collides with other players
		for (int j = 0; j < astronauts.size(); j++) {
			if (j == i) continue;
			collisionNorm = boundaries->spheresCollide(astronauts[i]->getCenter(), astronauts[i]->getRadius(), astronauts[j]->getCenter(), astronauts[j]->getRadius());
			if (glm::length(collisionNorm) > 0.01) {
				astronauts[i]->reflect(collisionNorm);
				break;
			}
		}
		if (glm::length(collisionNorm) > 0.01) continue;
		//if no collision, update accordingly
		astronauts[i]->update();
	}
	
	//updating the particles
	if (isSpawnPart) {
		spawnParticles->update();
	}
	if (isDespawnPart) {
		despawnParticles->update();
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glUseProgram(astroShaders);
	glUniform3fv(glGetUniformLocation(astroShaders, "eyePos"), 1, glm::value_ptr(eyePos));
	glUseProgram(0);
	// Render the objects
	if (glfwGetTime() > timeToSpawn) {
		timeToSpawn += 5.0 + (double)(rand() % 1000) / 100;
		spawnAstro();
	}
	if (glfwGetTime() > timeToDespawn) {
		timeToDespawn += 7.5 + (double)(rand() % 1000) / 100;
		despawnAstro();
	}
	light->draw(view, projection, astroShaders);
	lobby->draw(view, projection, shaderProgram);
	for (AstroStill* astro : astronauts) {
		astro->draw(view, projection, astroShaders);
	}

	if (isSpawnPart) {
		spawnParticles->draw(view, projection, particleShaders);
		if (glfwGetTime() - spawnParticles->getStart() > 3.0) {
			isSpawnPart = false;
			delete spawnParticles;
		}
	}
	if (isDespawnPart) {
		despawnParticles->draw(view, projection, particleShaders);
		if (glfwGetTime() - despawnParticles->getStart() > 3.0) {
			isDespawnPart = false;
			delete despawnParticles;
		}
	}
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */

	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
		case GLFW_KEY_W:
			direction = glm::vec3(0,0,-1);
			isMove = true;
			break;
		case GLFW_KEY_A:
			direction = glm::vec3(-1, 0, 0);
			isMove = true;
			break;
		case GLFW_KEY_S:
			direction = glm::vec3(0, 0, 1);
			isMove = true;
			break;
		case GLFW_KEY_D:
			direction = glm::vec3(1, 0, 0);
			isMove = true;
			break;
		default:
			break;
		}
	}
	if (action == GLFW_RELEASE)
		isMove = false;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		isHeld = true;
		glMatrixMode(GL_MODELVIEW);
		// some function/statements
	}
	else isHeld = false;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) { 
	if (isHeld) {
		float angleConst = 0.01;
		double displacement = ypos - lastPoint;
		double theta = angleConst * displacement;
		float oldZ = eyePos.z;
		float oldY = eyePos.y;
		eyePos.y = oldY * cos(theta) - oldZ * sin(theta);
		eyePos.z = oldY*sin(theta) + oldZ*cos(theta);
		view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
		lastPoint = ypos;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	float zoomScale = 0.5;
	glm::vec3 camVec = glm::normalize(lookAtPoint - eyePos);
	eyePos += zoomScale * (float)yoffset * camVec;
	view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
}

void Window::spawnAstro() {

	if (astronauts.size() == 10) return;
	AstroStill* ai = new AstroStill(getColor());
	spawnParticles = new ParticleSystem(ai->getCenter(), ai->getModel(), glfwGetTime(), true);
	isSpawnPart = true;
	astronauts.push_back(ai);
}

void Window::despawnAstro() {

	if (astronauts.size() <= 1) return;
	int astroIdx = rand() % (astronauts.size() - 1) + 1;
	available_colors.push_back(astronauts[astroIdx]->getColor());
	AstroStill* ai = astronauts[astroIdx];
	despawnParticles = new ParticleSystem(ai->getCenter(), ai->getModel(), glfwGetTime(), false);
	isDespawnPart = true;
	astronauts.erase(astronauts.begin() + astroIdx);
	delete ai;
}

glm::vec3 Window::getColor() {
	
	int colorIdx = rand() % available_colors.size();
	glm::vec3 color = available_colors[colorIdx];
	available_colors.erase(available_colors.begin() + colorIdx);
	return color;
}

// tutorial function for trackball mapping modified to work with glm
glm::vec3 Window::trackBallMapping(double x, double y) {
	glm::vec3 v;
	double d;
	v.x = (2.0 * x - Window::width) / Window::width;
	v.y = (Window::height - 2.0 * y) / Window::height;
	v.z = 0.0;
	d = glm::length(v);	
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrt(1.001 - d * d);
	glm::normalize(v);
	return v;
}