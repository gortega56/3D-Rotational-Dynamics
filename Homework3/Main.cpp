#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "CollisionResponse.h"

int main()
{
	CollisionResponse collisionResponse = CollisionResponse();
	Game::Launch(collisionResponse);
	return 0;
}
