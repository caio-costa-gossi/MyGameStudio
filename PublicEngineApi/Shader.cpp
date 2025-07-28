#include "Shader.h"

#include <glad/glad.h>

#include "Enums.h"
#include "SystemFileHelper.h"

Err Shader::Init(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath)
{
	glProgramId_ = glCreateProgram();

	vShaderPath_ = vShaderPath;
	fShaderPath_ = fShaderPath;
	gShaderPath_ = gShaderPath;

	vShaderId_ = glCreateShader(GL_VERTEX_SHADER);
	fShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);

	if (gShaderPath != nullptr)
		gShaderId_ = glCreateShader(GL_GEOMETRY_SHADER);

	return error_const::SUCCESS;
}

Err Shader::Build() const
{
	if (vShaderPath_ == nullptr || fShaderPath_ == nullptr)
		return error_const::PARAMETER_IS_NULL;

	// Compile Vertex Shader
	Err err = LoadCompileSource(vShaderPath_, vShaderId_);
	if (err.Code())
		return err;

	// Compile Geometry Shader
	if (gShaderPath_ != nullptr)
	{
		err = LoadCompileSource(gShaderPath_, gShaderId_);
		if (err.Code())
			return err;
	}

	// Compile Fragment Shader
	err = LoadCompileSource(fShaderPath_, fShaderId_);
	if (err.Code())
		return err;

	// Attach & Link
	err = AttachLinkShaders();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

void Shader::Use() const
{
	glUseProgram(glProgramId_);
}

int32_t Shader::GetId() const
{
	return glProgramId_;
}

bool Shader::IsInit() const
{
	return glProgramId_ > 0;
}

Err Shader::LoadCompileSource(const char* sourcePath, const int32_t& shaderSourceId)
{
	std::string shaderSource;

	Err err = SystemFileHelper::ReadFileString(sourcePath, shaderSource);
	if (err.Code())
		return err;

	const char* shaderSourceChar = shaderSource.data();
	glShaderSource(shaderSourceId, 1, &shaderSourceChar, nullptr);
	glCompileShader(shaderSourceId);

	int success;
	glGetShaderiv(shaderSourceId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderSourceId, 512, nullptr, infoLog);
		return Err(infoLog, error_const::SHADER_COMPILATION_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

Err Shader::AttachLinkShaders() const
{
	// Attach and link shaders
	glAttachShader(glProgramId_, vShaderId_);
	glAttachShader(glProgramId_, fShaderId_);

	if (gShaderId_ > 0) 
		glAttachShader(glProgramId_, gShaderId_);

	glLinkProgram(glProgramId_);

	// Delete shader objects
	glDeleteShader(vShaderId_);
	glDeleteShader(fShaderId_);

	if (gShaderId_ > 0)
		glDeleteShader(gShaderId_);

	// Check if successful
	int success;
	glGetProgramiv(glProgramId_, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(glProgramId_, 512, nullptr, infoLog);
		return Err(infoLog, error_const::SHADER_LINKING_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

void Shader::SetUniform(const char* uniformName, const float val) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);
	glUniform1f(uniformId, val);
}

void Shader::SetUniform(const char* uniformName, const float val1, const float val2) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);
	glUniform2f(uniformId, val1, val2);
}

void Shader::SetUniform(const char* uniformName, const float val1, const float val2, const float val3) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);
	glUniform3f(uniformId, val1, val2, val3);
}

void Shader::SetUniform(const char* uniformName, const float val1, const float val2, const float val3, const float val4) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);
	glUniform4f(uniformId, val1, val2, val3, val4);
}

void Shader::SetUniform(const char* uniformName, const enums::MatrixDim matrixDimension, const float* matrix, const bool transpose) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);

	switch (matrixDimension)
	{
	case enums::m2x2:
		glUniformMatrix2fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m3x3:
		glUniformMatrix3fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m4x4:
		glUniformMatrix4fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m2x3:
		glUniformMatrix2x3fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m3x2:
		glUniformMatrix3x2fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m4x3:
		glUniformMatrix4x3fv(uniformId, 1, transpose, matrix);
		break;
	case enums::m3x4:
		glUniformMatrix3x4fv(uniformId, 1, transpose, matrix);
		break;
	default:
		glUniformMatrix3fv(uniformId, 1, transpose, matrix);
	}
}

void Shader::SetUniform(const char* uniformName, const bool val) const
{
	const int32_t uniformId = glGetUniformLocation(glProgramId_, uniformName);
	glUniform1i(uniformId, val);
}