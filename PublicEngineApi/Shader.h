#pragma once
#include <unordered_map>

#include "Enums.h"
#include "Err.h"

struct CStrHash {
	uint32_t operator()(const std::string& s) const
	{
		const char* stringContent = s.c_str();

		uint32_t hash = 2166136261u;
		while (*stringContent) {
			hash ^= static_cast<uint8_t>(*stringContent++);
			hash *= 16777619u;
		}
		return hash;
	}
};

struct CStrEqual {
	bool operator()(const std::string& a, const std::string& b) const
	{
		return std::strcmp(a.c_str(), b.c_str()) == 0;
	}
};

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

	std::unordered_map<std::string, int32_t, CStrHash, CStrEqual> cache_;

	static Err LoadCompileSource(const char* sourcePath, const int32_t& shaderSourceId);
	Err AttachLinkShaders() const;

public:
	Shader() = default;
	Err Init(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr);

	Err Build() const;
	void Use() const;

	void SetUniform(const char* uniformName, float val);
	void SetUniform(const char* uniformName, float val1, float val2);
	void SetUniform(const char* uniformName, float val1, float val2, float val3);
	void SetUniform(const char* uniformName, float val1, float val2, float val3, float val4);
	void SetUniform(const char* uniformName, enums::MatrixDim matrixDimension,const float* matrix, bool transpose);
	void SetUniform(const char* uniformName, int32_t val);

	static void SetUniform(int32_t uniformId, float val);
	static void SetUniform(int32_t uniformId, float val1, float val2);
	static void SetUniform(int32_t uniformId, float val1, float val2, float val3);
	static void SetUniform(int32_t uniformId, float val1, float val2, float val3, float val4);
	static void SetUniform(int32_t uniformId, enums::MatrixDim matrixDimension, const float* matrix, bool transpose);
	static void SetUniform(int32_t uniformId, int32_t val);

	int32_t GetId() const;
	int32_t GetUniformId(const char* uniformName);
	bool IsInit() const;
};
