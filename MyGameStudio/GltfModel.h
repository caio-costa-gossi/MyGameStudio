#pragma once
#include <tiny_gltf.h>

struct Vec2
{
	float X;
	float Y;
};

struct Vec3
{
	float X;
	float Y;
	float Z;
};

struct Primitive
{
	int Type;
	int ComponentType;
	Vec3* Positions;
	Vec3* Normals;
	Vec3* Tangents;
};

struct Mesh
{
	std::vector<Primitive> Primitives;
};

struct Node
{
	Mesh Mesh;
	std::vector<double> Matrix;
	std::vector<double> Rotation;
	std::vector<double> Translation;
	std::vector<double> Scale;
};

class GltfModel
{
private:
	tinygltf::TinyGLTF loader_;
	tinygltf::Model model_;

	std::string err_;

	std::vector<Node> nodes_;

public:
	explicit GltfModel(const std::string& filepath);
};
