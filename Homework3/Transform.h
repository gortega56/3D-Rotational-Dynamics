#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>

using namespace glm;

class Transform
{
public:

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;
	
	vec3 Forward;
	vec3 Right;
	vec3 Up;

	Transform(vec3 position, vec3 rotation, vec3 scale);
	Transform();
	~Transform();

	mat4 GetWorldTransform();
	mat4 GetRotationTransform();
};

