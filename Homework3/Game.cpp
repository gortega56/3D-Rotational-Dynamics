#include "Game.h"
#include <stdlib.h>
#include "gl_log.h"
#include <stdio.h>

Game::Game()
{
	window = 0;
	monitor = 0;
	title = "Game";
}


Game::~Game()
{
}

void Game::Launch(Game& game)
{
	game.init();
	game.currentTime = glfwGetTime();
	while (!glfwWindowShouldClose(game.window)) {
		game.update(glfwGetTime());
		glfwSwapBuffers(game.window);
		glfwPollEvents();
		game.handleEvents(game.window);
	}
}

void Game::init()
{
	bool glfwSuccess = glfwInit();
	if (!glfwSuccess) {
		exit(1);
	}

	// Create Window
	monitor = glfwGetPrimaryMonitor();
	videoMode = glfwGetVideoMode(monitor);

	//GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "Sphere", NULL, NULL);
	window = glfwCreateWindow(800, 600, title, NULL, NULL);

	if (!window) {
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glewInit();


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	bool success = restart_gl_log();
	if (!success) {
		printf("Log failed\n");
	}
	// Subclass entry point for data init
}

void Game::update(double secondsElapsed)
{

}

void Game::handleEvents(GLFWwindow* window)
{

}

