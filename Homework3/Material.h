#pragma once
#include <GL\glew.h>
#include <iostream>
#include <unordered_map>
#include "Mesh.h"

using namespace std;

class Material
{
public:
	
	GLuint	VertexArrayID;
	GLuint	ShaderID;
	GLuint*	TextureIDs;

	Material();
	~Material();

	void SetShaderStage(const char* filename, GLenum shaderType);
	void SetShader();

	void BindMeshAttributes(const Mesh& mesh, const char* vertexAttribute, const char* normalAttribute, const char* tangentAttribute, const char* uvAttribute, const char* indexAttribute);
	void BindMeshAttributes(Mesh* mesh, GLuint* attributeIDs, char** attributes, int attributesCount);
	void BindUniformAttribute(const char* attribute);
	GLuint GetUniformAttribute(const char* attribute);

	void LoadTextures2D(const char** textureNames, int textureCount);

	inline int GetTextureCount() const { return TextureCount; };

private:
	unordered_map<const char*, GLuint>	uniformAttributeMap;
	unordered_map<GLenum, GLuint>		shaderMap;
	int TextureCount;
};

