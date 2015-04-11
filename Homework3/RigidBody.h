#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class RigidBody
{
public:
	vec3	velocity;
	vec3	angularVelocity;
	
	vec3	momentum;
	vec3	angularMomentum;

	quat	spin;

	float	mass;
	float	restitution;
	mat3	inertiaTensor;
	
	RigidBody();
	~RigidBody();
	void updateInertiaTensor(vec3 radialVector);
};

