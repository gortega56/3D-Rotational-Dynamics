#include "Material.h"
#include "gl_log.h"
#include <SOIL\SOIL.h>

Material::Material()
{
	VertexArrayID = 0;
	ShaderID = 0;
	TextureIDs = 0;
}


Material::~Material()
{
	delete[] TextureIDs;
}

void Material::SetShaderStage(const char* filename, GLenum shaderType)
{
	FILE* file;
	fopen_s(&file, filename, "r");
	char* shader = __nullptr;
	size_t charSize = sizeof(char);

	if (file) {
		fseek(file, 0, SEEK_END);											// Find the end of the file
		long size = ftell(file) + 1;										// Get the size

		fseek(file, 0, SEEK_SET);											// Set pointer to the beginning of the file
		shader = reinterpret_cast<char*>(malloc(charSize * size));			// Initialize buffer
		int charactersRead = fread(shader, charSize, size - 1, file);		// Read file to buffer
		shader[charactersRead] = '\0';										// Terminate at characters read... not size
	}

	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shader, NULL);
	glCompileShader(shaderID);

	GLint compileOK = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileOK);
	if (compileOK == GL_FALSE) {
		gl_log_error("ERROR: Shader[%i] did not compile\n", shaderID);
	}

	fclose(file);

	shaderMap.insert({ shaderType, shaderID });
}

void Material::SetShader()
{
	ShaderID = glCreateProgram();
	for (unordered_map<GLenum, GLuint>::iterator iter = shaderMap.begin(); iter != shaderMap.end(); iter++) {
		glAttachShader(ShaderID, iter->second);
	}

	int linkOK = GL_FALSE;
	glLinkProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &linkOK);
	if (linkOK == GL_FALSE) {
		//	gl_log_error("ERROR: Program[%i] did not link\n", shader_program);
	}
}

void Material::BindMeshAttributes(const Mesh& mesh, const char* vertexAttribute, const char* normalAttribute, const char* tangentAttribute, const char* uvAttribute, const char* indexAttribute)
{
	assert(vertexAttribute != NULL);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.VertexBufferID);
	GLuint attributeID = glGetAttribLocation(ShaderID, vertexAttribute);
	glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attributeID);

	if (normalAttribute != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.NormalBufferID);
		GLuint attributeID = glGetAttribLocation(ShaderID, normalAttribute);
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attributeID);
	}

	if (tangentAttribute != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.TangentBufferID);
		GLuint attributeID = glGetAttribLocation(ShaderID, tangentAttribute);
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attributeID);
	}

	if (uvAttribute != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.UVBufferID);
		GLuint attributeID = glGetAttribLocation(ShaderID, uvAttribute);
		glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attributeID);
	}

	if (indexAttribute != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.IndexBufferID);
		GLuint attributeID = glGetAttribLocation(ShaderID, indexAttribute);
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attributeID);
	}
}

void Material::BindMeshAttributes(Mesh* mesh, GLuint* attributeIDs, char** attributes, int attributesCount)
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBufferID);
	GLuint attributeID = glGetAttribLocation(ShaderID, "texCoord");
	glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attributeID);

	for (int attrIndex = 0; attrIndex < attributesCount; attrIndex++) {
		glBindBuffer(GL_ARRAY_BUFFER, attributeIDs[attrIndex]);

		GLuint attributeID = glGetAttribLocation(ShaderID, attributes[attrIndex]);
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attributeID);
	}

	
}

void Material::BindUniformAttribute(const char* attribute)
{
	GLuint attributeID = glGetUniformLocation(ShaderID, attribute);
	uniformAttributeMap.insert({ attribute, attributeID });
}

GLuint Material::GetUniformAttribute(const char* attribute)
{
	return uniformAttributeMap.at(attribute);
}

void Material::LoadTextures2D(const char** textureNames, int textureCount)
{
	TextureIDs = new GLuint[textureCount];
	TextureCount = textureCount;
	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < textureCount; i++) {
		TextureIDs[i] = SOIL_load_OGL_texture(textureNames[i], SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		glBindTexture(GL_TEXTURE_2D, TextureIDs[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}