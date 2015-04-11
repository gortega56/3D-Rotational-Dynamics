#pragma once
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Geometry.h"

class Collider;

using namespace std;

class GameObject
{
public:

	glm::vec3					color;

	Transform*					transform;
	Geometry*					geometry;
	RigidBody*					rigidBody;
	Mesh*						mesh;
	Material*					material;
	Collider*					collider;

	GameObject();
	~GameObject();
};

