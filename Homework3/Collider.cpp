#include "Collider.h"
#include "GameObject.h"

Collider::Collider(GameObject* gameObject) : gameObject(gameObject), VertexCount(gameObject->geometry->GetVertexCount())
{
	halfScale = gameObject->transform->Scale * 0.5f;
	xAxis = { 1.0f, 0.0f, 0.0f };
	yAxis = { 0.0f, 1.0f, 0.0f };
	zAxis = { 0.0f, 0.0f, 1.0f };

	Vertices = new vec3[VertexCount];
	//memcpy(Vertices, gameObject->geometry->Vertices, sizeof(gameObject->geometry->Vertices[0]) * VertexCount);
}

Collider::Collider()
{
}


Collider::~Collider()
{
	gameObject = NULL;
}

void Collider::UpdateGeometry()
{
	mat4 transform = gameObject->transform->GetWorldTransform();
	min = vec3(FLT_MAX);
	max = vec3(-FLT_MAX);

	for (int i = 0; i < VertexCount; i++) {
		Vertices[i] = vec3(transform * vec4(gameObject->geometry->Vertices[i], 1.0f));
		min = glm::min(min, vec3(Vertices[i]));
		max = glm::max(max, vec3(Vertices[i]));
	}

	mid = (min + max) * 0.5f;
	xAxis = normalize(mat3(transform) * vec3(1.0f, 0.0f, 0.0f));
	yAxis = normalize(mat3(transform) * vec3(0.0f, 1.0f, 0.0f));
	zAxis = normalize(mat3(transform) * vec3(0.0f, 0.0f, 1.0f));
}