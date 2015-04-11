#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

using namespace std;

class Game
{
public:
	double				currentTime;
	GLFWwindow*			window;
	GLFWmonitor*		monitor;
	const GLFWvidmode*	videoMode;
	const char*			title;

	Game();
	~Game();

	static void Launch(Game& game);
	virtual void init(); // Call superClass init in subclass.
	virtual void update(double secondsElapsed);
	virtual void handleEvents(GLFWwindow* window);
};

