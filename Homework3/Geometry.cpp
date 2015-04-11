#include "Geometry.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include <vector>

Geometry::Geometry(vec3* vertices, int vertexCount, vec3* normals, int normalCount, vec3* tangents, int tangentCount, vec2* uvs, int uvCount, int* indices, int indexCount)
	: VertexCount(vertexCount), NormalCount(normalCount), TangentCount(tangentCount), UVCount(uvCount), IndexCount(indexCount)
{
	assert(VertexCount > 0);

	Vertices = new vec3[VertexCount];
	memcpy(Vertices, vertices, sizeof(vec3) * VertexCount);

	if (NormalCount > 0){
		Normals = new vec3[NormalCount];
		memcpy(Normals, normals, sizeof(vec3) * NormalCount);
	}

	if (TangentCount > 0) {
		Tangents = new vec3[TangentCount];
		memcpy(Tangents, tangents, sizeof(vec3) * TangentCount);
	}

	if (UVCount > 0) {
		UVs = new vec2[UVCount];
		memcpy(UVs, uvs, sizeof(vec2) * UVCount);
	}

	if (IndexCount > 0) {
		Indices = new int[IndexCount];
		memcpy(Indices, indices, sizeof(int) * IndexCount);
	}
}

Geometry::Geometry(const char* filename) {}

Geometry::Geometry() {}


Geometry::~Geometry() 
{
	delete[] Vertices;
	delete[] Normals;
	delete[] Tangents;
	delete[] UVs;
	delete[] Indices;
}

void Geometry::GetMinMaxVertices(const mat4& transform, vec3* min, vec3* max)
{
	*min = vec3(FLT_MAX);
	*max = vec3(-FLT_MAX);

	for (int vIndex = 0; vIndex < VertexCount; vIndex++) {
		vec4 worldVertex = transform * vec4(Vertices[vIndex], 1.0f);
		*min = glm::min(*min, vec3(worldVertex));
		*max = glm::max(*max, vec3(worldVertex));
	}
}

Geometry* Geometry::Sphere(int stackCount, int sliceCount)
{
	std::vector<float> phiCoordinates;
	float phiStep = M_PI / stackCount;
	for (int i = 0; i < stackCount + 1; i++)
	{
		float value = -M_PI_2 + (i * phiStep);
		phiCoordinates.push_back(-M_PI_2 + (i * phiStep));
	}

	std::vector<float> thetaCoordinates;
	float thetaStep = (M_PI * 2) / (sliceCount * 2);
	for (int i = 0; i < (sliceCount * 2) + 1; i++)
	{
		thetaCoordinates.push_back(0 + (i * thetaStep));
	}

	std::vector<vec3> vertices;
	float radius = 0.5f;
	for (int i = 0; i < phiCoordinates.size() - 1; i++)
	{
		for (int j = 0; j < thetaCoordinates.size() - 1; j++)
		{
			vec3 vertex1 = vec3(radius * cosf(phiCoordinates[i]) * sinf(thetaCoordinates[j]), radius * sinf(phiCoordinates[i]) * sinf(thetaCoordinates[j]), radius * cosf(thetaCoordinates[j]));
			vec3 vertex2 = vec3(radius * cosf(phiCoordinates[i]) * sinf(thetaCoordinates[j + 1]), radius * sinf(phiCoordinates[i]) * sinf(thetaCoordinates[j + 1]), radius * cosf(thetaCoordinates[j + 1]));
			vec3 vertex3 = vec3(radius * cosf(phiCoordinates[i + 1]) * sinf(thetaCoordinates[j + 1]), radius * sinf(phiCoordinates[i + 1]) * sinf(thetaCoordinates[j + 1]), radius * cosf(thetaCoordinates[j + 1]));
			vec3 vertex4 = vec3(radius * cosf(phiCoordinates[i + 1]) * sinf(thetaCoordinates[j]), radius * sinf(phiCoordinates[i + 1]) * sinf(thetaCoordinates[j]), radius * cosf(thetaCoordinates[j]));

			if (thetaCoordinates[j] <= M_PI)
			{
				vertices.push_back(vertex1);
				vertices.push_back(vertex2);
				vertices.push_back(vertex3);

				vertices.push_back(vertex1);
				vertices.push_back(vertex3);
				vertices.push_back(vertex4);
			}

			if (thetaCoordinates[j] >= M_PI)
			{
				vertices.push_back(vertex1);
				vertices.push_back(vertex3);
				vertices.push_back(vertex2);

				vertices.push_back(vertex1);
				vertices.push_back(vertex4);
				vertices.push_back(vertex3);
			}
		}
	}

	Geometry* geometry = new Geometry(&vertices[0], vertices.size(), NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	return geometry;
}

Geometry* Geometry::Cube()
{
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	vec3 vertex1 = vec3(-0.5f, 0.5f, 0.5f);
	vec3 vertex2 = vec3(0.5f, 0.5f, 0.5f);
	vec3 vertex3 = vec3(0.5f, -0.5f, 0.5f);
	vec3 vertex4 = vec3(-0.5f, -0.5f, 0.5f);
	vec3 vertex5 = vec3(-0.5f, 0.5f, -0.5f);
	vec3 vertex6 = vec3(0.5f, 0.5f, -0.5f);
	vec3 vertex7 = vec3(0.5f, -0.5f, -0.5f);
	vec3 vertex8 = vec3(-0.5f, -0.5f, -0.5f);

	// Front
	vec3 front = { 0.0f, 0.0, 1.0 };
	vertices.push_back(vertex1);
	normals.push_back(front);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex2);
	normals.push_back(front);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex3);
	normals.push_back(front);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex1);
	normals.push_back(front);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex3);
	normals.push_back(front);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex4);
	normals.push_back(front);
	uvs.push_back({ 0.0f, 1.0f });

	// Back
	vec3 back = { 0.0f, 0.0, -1.0 };
	vertices.push_back(vertex5);
	normals.push_back(back);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(back);
	uvs.push_back({ 0.0f, 1.0f });

	vertices.push_back(vertex6);
	normals.push_back(back);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex5);
	normals.push_back(back);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex8);
	normals.push_back(back);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(back);
	uvs.push_back({ 0.0f, 1.0f });

	// Left
	vec3 left = { -1.0f, 0.0, 0.0 };
	vertices.push_back(vertex5);
	normals.push_back(left);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex1);
	normals.push_back(left);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex4);
	normals.push_back(left);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex5);
	normals.push_back(left);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex4);
	normals.push_back(left);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex8);
	normals.push_back(left);
	uvs.push_back({ 0.0f, 1.0f });

	// Right
	vec3 right = { 1.0f, 0.0, 0.0 };

	vertices.push_back(vertex2);
	normals.push_back(right);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex6);
	normals.push_back(right);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(right);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex2);
	normals.push_back(right);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(right);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex3);
	normals.push_back(right);
	uvs.push_back({ 0.0f, 1.0f });

	// Top 
	vec3 top = { 0.0f, 1.0, 0.0 };
	vertices.push_back(vertex5);
	normals.push_back(top);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex6);
	normals.push_back(top);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex2);
	normals.push_back(top);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex5);
	normals.push_back(top);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex2);
	normals.push_back(top);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex1);
	normals.push_back(top);
	uvs.push_back({ 0.0f, 1.0f });

	// Bottom
	vec3 bottom = { 0.0f, -1.0, 0.0 };
	vertices.push_back(vertex4);
	normals.push_back(bottom);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex3);
	normals.push_back(bottom);
	uvs.push_back({ 1.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(bottom);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex4);
	normals.push_back(bottom);
	uvs.push_back({ 0.0f, 0.0f });

	vertices.push_back(vertex7);
	normals.push_back(bottom);
	uvs.push_back({ 1.0f, 1.0f });

	vertices.push_back(vertex8);
	normals.push_back(bottom);
	uvs.push_back({ 0.0f, 1.0f });

	Geometry* geometry = new Geometry(&vertices[0], vertices.size(), &normals[0], normals.size(), NULL, 0, &uvs[0], uvs.size(), NULL, 0);
	return geometry;
}

Geometry* Geometry::Square()
{
	std::vector<vec3> vertices;
	vertices.push_back({ -0.5, 0.5, 0.0 });
	vertices.push_back({ -0.5, -0.5, 0.0 });
	vertices.push_back({ 0.5, 0.5, 0.0 });
	vertices.push_back({ 0.5, 0.5, 0.0 });
	vertices.push_back({ -0.5, -0.5, 0.0 });
	vertices.push_back({ 0.5, -0.5, 0.0 });

	Geometry* geometry = new Geometry(&vertices[0], vertices.size(), NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	return geometry;
}

Geometry* Geometry::Circle()
{
	std::vector<vec3> vertices;
	vertices.push_back(vec3(0));
	float radius = 0.5f;
	for (int angle = 0; angle <= 360; angle += 15) {
		vertices.push_back(vec3(radius * cosf(radians((float)angle)), radius * sinf(radians((float)angle)), 0.0f));
	}

	Geometry* geometry = new Geometry(&vertices[0], vertices.size(), NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	return geometry;
}

Geometry* Geometry::Triangle()
{
	std::vector<vec3> vertices;
	vertices.push_back({ 0.0, 0.5, 0.0 });
	vertices.push_back({ -0.5, -0.5, 0.0 });
	vertices.push_back({ 0.5, -0.5, 0.0 });

	Geometry* geometry = new Geometry(&vertices[0], vertices.size(), NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	return geometry;
}