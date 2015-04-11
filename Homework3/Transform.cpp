#include "Transform.h"


Transform::Transform(vec3 position, vec3 rotation, vec3 scale) : 
	Position(position),
	Rotation(rotation),
	Scale(scale),
	Forward({ 0.0f, 0.0f, 1.0f }),
	Right({ 1.0f, 0.0f, 0.0f }),
	Up({ 0.0f, 1.0f, 0.0f }) {}

Transform::Transform() : Transform(vec3(0.0f), vec3(0.0f), vec3(1.0f)) {}


Transform::~Transform() {}

mat4 Transform::GetWorldTransform()
{
	mat4 identity = mat4(1.0f);
	return translate(identity, Position) *
			GetRotationTransform() *
			glm::scale(identity, Scale);
}

mat4 Transform::GetRotationTransform()
{
	mat4 identity = mat4(1.0f);
	return rotate(identity, Rotation.z, vec3(0, 0, 1)) *
			rotate(identity, Rotation.y, vec3(0, 1, 0)) *
			rotate(identity, Rotation.x, vec3(1, 0, 0));
}