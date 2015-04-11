#pragma once
#include <glm\glm.hpp>
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include "GameObject.h"

using namespace glm;

class Collider;

struct Collision
{
	GameObject* a;
	GameObject* b;
	vec3 normal;
	float MTD;
	vec3 contactPoint;
};


class CollisionController
{
public:
	vec3*	CapturedColliderPositions;
	int		CapturedColliderPositionsCount;
	
	CollisionController();
	~CollisionController();

	void CaptureGeometry(GameObject** gameObjects, int gameObjectsCount);
	void DispatchCollisions(GameObject** gameObjects, int gameObjectsCount);
	bool hasSeparatingAxis(const Collider& a, const Collider& b, Collision* collision);
	bool isSeparatingAxis(vec3 axis, const Collider& a, const Collider& b, Collision* collision);
	bool GetEdgeToEdgeIntersection(vec3 a1, vec3 a2, vec3 b1, vec3 b2, vec3* intersection);
	void ResolveCollision(const Collider& a, const Collider& b, const Collision& collision);

private:
	int EvaluateCollisionAccuracy(const const Collider& a, const Collider& b, Collision* collisions, int collisionCount);
};

