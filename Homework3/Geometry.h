#pragma once
#include <glm\glm.hpp>

using namespace glm;

class Geometry
{
public:

	vec3*	Vertices;
	vec3*	Normals;
	vec3*	Tangents;
	vec2*	UVs;
	int*	Indices;

	Geometry(vec3* vertices, int vertexCount, vec3* normals, int normalCount, vec3* tangents, int tangentCount, vec2* uvs, int uvCount, int* indices, int indexCount);
	Geometry(const char* filename);
	Geometry();
	~Geometry();

	void GetMinMaxVertices(const mat4& transform, vec3* min, vec3* max);

	inline int GetVertexCount() const { return VertexCount; };
	inline int GetNormalCount() const { return NormalCount; };
	inline int GetTangentCount() const { return TangentCount; };
	inline int GetUVCount() const { return UVCount; };
	inline int GetIndexCount() const { return IndexCount; };

	static Geometry* Sphere(int stackCount, int sliceCount);
	static Geometry* Cube();
	static Geometry* Square();
	static Geometry* Circle(); // Use Triangle Fan
	static Geometry* Triangle();

private:
	int		VertexCount;
	int		NormalCount;
	int		TangentCount;
	int		UVCount;
	int		IndexCount;
};

