#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Geometry.h"

class Mesh
{
public:

	GLuint  VertexBufferID;
	GLuint  NormalBufferID;
	GLuint	TangentBufferID;
	GLuint	UVBufferID;
	GLuint	IndexBufferID;

	GLenum	PolygonMode;
	GLenum	RenderMode;

	Mesh(const Geometry& geometry);
	Mesh(const Mesh& other);
	Mesh();
	~Mesh();

	inline int	GetVertexCount() const { return VertexCount; };
	inline int	GetIndexCount() const { return IndexCount; };

private:
	int		VertexCount;
	int		IndexCount;

	void SetBufferData(void* data, int size, GLuint* ID);
};
