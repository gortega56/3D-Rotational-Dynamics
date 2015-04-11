#pragma once
#include <glm\glm.hpp>

using namespace glm;

class GameObject;

class Collider
{
public:
	vec3	min;
	vec3	mid;
	vec3	max;
	vec3	xAxis;
	vec3	yAxis;
	vec3	zAxis;
	vec3	halfScale;

	GameObject* gameObject;
	vec3*		Vertices;

	Collider(GameObject* gameObject);
	Collider();
	~Collider();

	void UpdateGeometry();

	inline int GetVertexCount() const { return VertexCount; };

private:
	int		VertexCount;
};

