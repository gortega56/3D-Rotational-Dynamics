#include "RigidBody.h"


RigidBody::RigidBody() : 
	velocity({ 0.0f, 0.0f, 0.0f }), 
	angularVelocity({ 0.0f, 0.0f, 0.0f }),
	momentum({ 0.0f, 0.0f, 0.0f }), 
	angularMomentum({ 0.0f, 0.0f, 0.0f }),
	spin({ 0.0f, 0.0f, 0.0f, 1.0f }),
	mass(1.f),
	restitution(0.2f),
	inertiaTensor(mat3(1.0f))
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::updateInertiaTensor(vec3 radialVector)
{
	inertiaTensor *= (mass * (radialVector.x * radialVector.x)) / 6.0f;
}
