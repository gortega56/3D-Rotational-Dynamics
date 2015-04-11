#include "Mesh.h"
#define _USE_MATH_DEFINES 
#include <math.h>

Mesh::Mesh(const Geometry& geometry) : VertexCount(geometry.GetVertexCount()), IndexCount(geometry.GetIndexCount())
{
	assert(VertexCount > 0);
	SetBufferData(geometry.Vertices, sizeof(geometry.Vertices[0]) * VertexCount, &VertexBufferID);

	int normalCount = geometry.GetNormalCount();
	if (normalCount > 0) {
		SetBufferData(geometry.Normals, sizeof(geometry.Normals[0]) * normalCount, &NormalBufferID);
	}

	int tangentCount = geometry.GetTangentCount();
	if (tangentCount > 0) {
		SetBufferData(geometry.Tangents, sizeof(geometry.Tangents[0]) * tangentCount, &TangentBufferID);
	}

	int uvCount = geometry.GetUVCount();
	if (uvCount > 0) {
		SetBufferData(geometry.UVs, sizeof(geometry.UVs[0]) * uvCount, &UVBufferID);
	}

	if (IndexCount > 0) {
		SetBufferData(geometry.Indices, sizeof(geometry.Indices[0]) * IndexCount, &IndexBufferID);
	}
}

void Mesh::SetBufferData(void* data, int size, GLuint* ID)
{
	glGenBuffers(1, ID);
	glBindBuffer(GL_ARRAY_BUFFER, *ID); // OpenGL.GL_Array_Buffer = buffer with ID(vertexBufferID)
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

Mesh::Mesh()
{
	VertexBufferID = 0;
	NormalBufferID = 0;
	PolygonMode = GL_FILL;
	RenderMode = GL_POINTS;
}

Mesh::~Mesh()
{

}