#pragma once
#include "Game.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "CollisionController.h"
#include "Camera.h"

class CollisionResponse :
	public Game
{
public:
	Camera*								camera;
	GameObject*							boundingBox;
	vec3								boundingBoxMin;
	vec3								boundingBoxMax;

	Material*							material;

	vector<GameObject*>					gameObjects;
	int									gameObjectsCount;
	vec3								color;
	vec3								collisionColor;
	vec3								boundingBoxColor;
	CollisionController					collisionController;

	CollisionResponse();
	~CollisionResponse();

	virtual void init(); // Call superClass init in subclass.
	virtual void update(double secondsElapsed);
	virtual void handleEvents(GLFWwindow* window);
	
	void detectAndResolveBoundingBoxCollisions();
	void detectEllasticCollisions();
	void performIntegration(double secondsElapsed);
};

