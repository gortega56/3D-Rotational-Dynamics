#include "CollisionController.h"
#include "Collider.h"
#include <vector>

CollisionController::CollisionController()
{
	CapturedColliderPositions = __nullptr;
	CapturedColliderPositionsCount = 0;
}


CollisionController::~CollisionController()
{
	delete[] CapturedColliderPositions;
}

void CollisionController::CaptureGeometry(GameObject** gameObjects, int gameObjectsCount)
{
	if (CapturedColliderPositions == __nullptr) {
		CapturedColliderPositions = new vec3[gameObjectsCount];
		CapturedColliderPositionsCount = gameObjectsCount;
	}

	for (int i = 0; i < gameObjectsCount; i++) {
		CapturedColliderPositions[i] = gameObjects[i]->transform->Position;
	}
}

void CollisionController::DispatchCollisions(GameObject** gameObjects, int gameObjectsCount)
{
	for (int i = 0; i < gameObjectsCount; i++) {
		gameObjects[i]->collider->UpdateGeometry();
	}
	
	for (int i = 0; i < gameObjectsCount; i++) {
		for (int j = i + 1; j < gameObjectsCount; j++) {
			float previousDistance = length2(CapturedColliderPositions[j] - CapturedColliderPositions[i]);
			float currentDistance = length2(gameObjects[j]->transform->Position - gameObjects[i]->transform->Position);
			if (previousDistance < currentDistance) {
				// Objects are moving away. Don't test for collision.
				continue;
			}

			Collision collision;
			if (!hasSeparatingAxis(*gameObjects[i]->collider, *gameObjects[j]->collider, &collision)) {
				ResolveCollision(*gameObjects[i]->collider, *gameObjects[j]->collider, collision);
			}
		}
	}
}

bool CollisionController::hasSeparatingAxis(const Collider& a, const Collider& b, Collision* collision)
{
	Collision aCollisions[15];
	// Case 1
	if (isSeparatingAxis(a.xAxis, a, b, &aCollisions[0])) {
		return true;
	} 	// Case 2
	else if (isSeparatingAxis(a.yAxis, a, b, &aCollisions[1])) {
		return true;
	}	// Case 3
	else if (isSeparatingAxis(a.zAxis, a, b, &aCollisions[2])) {
		return true;
	} 	// Case 4
	else if (isSeparatingAxis(b.xAxis, a, b, &aCollisions[3])) {
		return true;
	}	// Case 5
	else if (isSeparatingAxis(b.yAxis, a, b, &aCollisions[4])) {
		return true;
	} 	// Case 6
	else if (isSeparatingAxis(b.zAxis, a, b, &aCollisions[5])) {
		return true;
	}	// Case 7
	else if (isSeparatingAxis(cross(a.xAxis, b.xAxis), a, b, &aCollisions[6])) {
		return true;
	} 	// Case 8
	else if (isSeparatingAxis(cross(a.xAxis, b.yAxis), a, b, &aCollisions[7])) {
		return true;
	}	// Case 9
	else if (isSeparatingAxis(cross(a.xAxis, b.zAxis), a, b, &aCollisions[8])) {
		return true;
	} 	// Case 10
	else if (isSeparatingAxis(cross(a.yAxis, b.xAxis), a, b, &aCollisions[9])) {
		return true;
	}	// Case 11
	else if (isSeparatingAxis(cross(a.yAxis, b.yAxis), a, b, &aCollisions[10])) {
		return true;
	} 	// Case 12
	else if (isSeparatingAxis(cross(a.yAxis, b.zAxis), a, b, &aCollisions[11])) {
		return true;
	}	// Case 13
	else if (isSeparatingAxis(cross(a.zAxis, b.xAxis), a, b, &aCollisions[12])) {
		return true;
	} 	// Case 14
	else if (isSeparatingAxis(cross(a.zAxis, b.yAxis), a, b, &aCollisions[13])) {
		return true;
	}	// Case 15
	else if (isSeparatingAxis(cross(a.zAxis, b.zAxis), a, b, &aCollisions[14])) {
		return true;
	}
	else {

		int index = EvaluateCollisionAccuracy(a, b, aCollisions, 15);
		*collision = aCollisions[index];
		return false;
	}
}

bool CollisionController::isSeparatingAxis(vec3 axis, const Collider& a, const Collider& b, Collision* collision)
{
	vec3 distance = vec3(b.mid - a.mid);
	float T = fabsf(dot(distance, axis));
	float shapeProjection = fabsf(dot(a.halfScale.x * a.xAxis, axis)) + fabsf(dot(a.halfScale.y * a.yAxis, axis)) + fabsf(dot(a.halfScale.z * a.zAxis, axis)) +
		fabsf(dot(b.halfScale.x * b.xAxis, axis)) + fabsf(dot(b.halfScale.y * b.yAxis, axis)) + fabsf(dot(b.halfScale.z * b.zAxis, axis));
	if (T > shapeProjection) {
		return true;
	}

	collision->MTD = shapeProjection - T;
	collision->normal = axis;
	return false;
}

int CollisionController::EvaluateCollisionAccuracy(const const Collider& a, const Collider& b, Collision* collisions, int collisionCount)
{
	float minimumTranslationDistance = FLT_MAX;
	int bestCase;
	for (int i = 0; i < collisionCount; i++) {
		if (length(collisions[i].normal) < FLT_EPSILON) {
			continue;
		}

		if (collisions[i].MTD < minimumTranslationDistance) {
			minimumTranslationDistance = collisions[i].MTD;
			bestCase = i;
		}
	}

	vec3 AToB = b.mid - a.mid;
	if (dot(collisions[bestCase].normal, AToB) < 0) {
		collisions[bestCase].normal *= -1;
	}

	if (bestCase < 6) {
		// Face based Contact
		if (bestCase < 3) {
			float minProjection = FLT_MAX;
			for (int i = 0; i < 8; i++) {
				float projectionB = dot(b.Vertices[i], AToB);
				if (projectionB < minProjection) {
					minProjection = projectionB;
					collisions[bestCase].contactPoint = b.Vertices[i];
				}
			}
		}
		else  {
			float maxProjection = FLT_MIN;
			for (int i = 0; i < 8; i++) {
				float projectionA = dot(a.Vertices[i], AToB);
				if (projectionA > maxProjection) {
					maxProjection = projectionA;
					collisions[bestCase].contactPoint = a.Vertices[i];
				}
			}
		}
	}
	else {
		// Edge based Contact

		// Get edge on each shape
		pair<vec3, vec3> edgeA, edgeB;

		float nearestDistanceToA, nearestDistanceToB;
		nearestDistanceToA = nearestDistanceToB = FLT_MAX;
		for (int i = 0; i < 8; i++) {
			float vertexAToBMid = length2(b.mid - a.Vertices[i]);
			if (vertexAToBMid < nearestDistanceToB) {
				nearestDistanceToB = vertexAToBMid;
				edgeA.second = edgeA.first;
				edgeA.first = a.Vertices[i];
			}

			float vertexBToAMid = length2(a.mid - b.Vertices[i]);
			if (vertexBToAMid < nearestDistanceToA) {
				nearestDistanceToA = vertexBToAMid;
				edgeB.second = edgeB.first;
				edgeB.first = b.Vertices[i];
			}
		}

		bool s = GetEdgeToEdgeIntersection(edgeA.first, edgeA.second, edgeB.first, edgeB.second, &collisions[bestCase].contactPoint);
	}

	return bestCase;
}

bool CollisionController::GetEdgeToEdgeIntersection(vec3 a1, vec3 a2, vec3 b1, vec3 b2, vec3* intersection)
{
	vec3 edgeA = a2 - a1;
	vec3 edgeB = b2 - b1;
	vec3 BToA = a1 - b1;

	if (length(cross(edgeA, edgeB)) < FLT_EPSILON) {
		return false;
	}

	vec3 aCrossB = cross(edgeA, edgeB);
	float aCrossBLength2 = length2(aCrossB);
	float t1 = dot(cross(BToA, edgeB), aCrossB) / aCrossBLength2;
	float t2 = dot(cross(BToA, edgeA), aCrossB) / aCrossBLength2;

	vec3 closestEdgePointA = a1 + t1 * edgeA;
	vec3 closestEdgePointB = b1 + t2 * edgeB;

	*intersection = (closestEdgePointA + closestEdgePointB) * 0.5f;
	return true;
}

void CollisionController::ResolveCollision(const Collider& a, const Collider& b, const Collision& collision)
{
	b.gameObject->transform->Position += collision.normal * collision.MTD;

	vec3 r1 = a.mid - collision.contactPoint;
	vec3 r2 = b.mid - collision.contactPoint;

	vec3 u1 = a.gameObject->rigidBody->velocity + cross(a.gameObject->rigidBody->angularVelocity, r1);
	vec3 u2 = b.gameObject->rigidBody->velocity + cross(b.gameObject->rigidBody->angularVelocity, r2);
	vec3 uRel = u1 - u2;

	mat3 R1 = mat3(a.gameObject->transform->GetRotationTransform());
	mat3 R2 = mat3(b.gameObject->transform->GetRotationTransform());

	mat3 J1 = R1 * inverse(a.gameObject->rigidBody->inertiaTensor) * transpose(R1);
	mat3 J2 = R2 * inverse(b.gameObject->rigidBody->inertiaTensor) * transpose(R2);

	float e = (a.gameObject->rigidBody->restitution + b.gameObject->rigidBody->restitution) * 0.5f;
	float inverseM1 = (1 / a.gameObject->rigidBody->mass);
	float inverseM2 = (1 / b.gameObject->rigidBody->mass);

	float j1 = a.gameObject->rigidBody->mass * ((r1.x * r1.x) + (r1.y * r1.y) + (r1.z * r1.z));
	float j2 = b.gameObject->rigidBody->mass * ((r2.x * r2.x) + (r2.y * r2.y) + (r2.z * r2.z));

	vec3 J1r1CrossN = J1 * cross(r1, collision.normal);
	vec3 J2r2CrossN = J2 * cross(r2, collision.normal);

	float k =((e + 1) * dot(uRel, collision.normal)) / (dot(((inverseM1 + inverseM2) * collision.normal) + cross(J1r1CrossN, r1) + cross(J2r2CrossN, r2), collision.normal));

	a.gameObject->rigidBody->velocity -= (k * collision.normal) * inverseM1;
	b.gameObject->rigidBody->velocity += (k * collision.normal) * inverseM2;

	a.gameObject->rigidBody->angularVelocity -= cross(r1, (k * collision.normal)) / j1;
	b.gameObject->rigidBody->angularVelocity += cross(r2, (k * collision.normal)) / j2;
}
