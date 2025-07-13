#pragma once
#include "Err.h"

class Shader
{
private:
	int32_t glProgramId_ = -1;

	const char* vShaderPath_ = nullptr;
	const char* fShaderPath_ = nullptr;
	const char* gShaderPath_ = nullptr;

	int32_t vShaderId_ = -1;
	int32_t gShaderId_ = -1;
	int32_t fShaderId_ = -1;

	static Err LoadCompileSource(const char* sourcePath, const int32_t& shaderSourceId);
	Err AttachLinkShaders() const;

public:
	Shader() = default;
	Err Init(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr);

	Err Build() const;
	void Use() const;

	void SetUniform(const char* uniformName, float val) const;
	void SetUniform(const char* uniformName, float val1, float val2) const;
	void SetUniform(const char* uniformName, float val1, float val2, float val3) const;
	void SetUniform(const char* uniformName, float val1, float val2, float val3, float val4) const;

	int32_t GetId() const;
	bool IsInit() const;
};
