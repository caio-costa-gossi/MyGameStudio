#include "Serialization.h"

#include "DataReader.h"
#include "../MyGameStudio/DataStream.h"

DataStream Serialization::SerializeMesh(const Mesh& mesh, uint64_t& resultSize)
{
	uint64_t objectSize = sizeof(mesh.IndexCount) + sizeof(mesh.MeshId) + sizeof(mesh.VertexCount) + sizeof(mesh.Material);
	objectSize += sizeof(mesh.HorizontalWrap) + sizeof(mesh.VerticalWrap);
	objectSize += mesh.IndexCount * sizeof(mesh.IndexList.get()[0]);
	objectSize += mesh.VertexCount * sizeof(mesh.VertexList.get()[0]);

	resultSize = objectSize;
	DataStream stream(objectSize);

	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.IndexCount), sizeof(mesh.IndexCount));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.MeshId), sizeof(mesh.MeshId));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.VertexCount), sizeof(mesh.VertexCount));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.HorizontalWrap), sizeof(mesh.HorizontalWrap));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.VerticalWrap), sizeof(mesh.VerticalWrap));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.Material), sizeof(mesh.Material));

	stream.Write(reinterpret_cast<const uint8_t*>(mesh.IndexList.get()), mesh.IndexCount * sizeof(mesh.IndexList.get()[0]));
	stream.Write(reinterpret_cast<const uint8_t*>(mesh.VertexList.get()), mesh.VertexCount * sizeof(mesh.VertexList.get()[0]));

	return stream;
}

Mesh Serialization::DesserializeMesh(const uint8_t* data, const uint64_t dataSize)
{
	DataReader reader(data, dataSize);
	Mesh mesh = { };

	reader.Read(reinterpret_cast<uint8_t*>(&mesh.IndexCount), sizeof(mesh.IndexCount));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.MeshId), sizeof(mesh.MeshId));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.VertexCount), sizeof(mesh.VertexCount));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.HorizontalWrap), sizeof(mesh.HorizontalWrap));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.VerticalWrap), sizeof(mesh.VerticalWrap));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.Material), sizeof(mesh.Material));

	mesh.IndexList = std::make_unique<uint32_t[]>(mesh.IndexCount);
	mesh.VertexList = std::make_unique<Vertex[]>(mesh.VertexCount);

	reader.Read(reinterpret_cast<uint8_t*>(mesh.IndexList.get()), mesh.IndexCount * sizeof(mesh.IndexList.get()[0]));
	reader.Read(reinterpret_cast<uint8_t*>(mesh.VertexList.get()), mesh.VertexCount * sizeof(mesh.VertexList.get()[0]));

	return mesh;
}

DataStream Serialization::SerializeModel(const Model& model, uint64_t& resultSize)
{
	// Serialize with additional metadata for each mesh size, after model.MeshCount and before model.Meshes
	uint64_t totalModelSize = sizeof(model.ModelId) + sizeof(model.MeshCount);
	const uint64_t offsetDataSize = model.MeshCount * sizeof(uint64_t);
	totalModelSize += offsetDataSize;

	// Mesh offsets & Mesh Data
	std::unique_ptr<uint64_t[]> offsets = std::make_unique<uint64_t[]>(model.MeshCount);
	std::unique_ptr<DataStream[]> meshData = std::make_unique<DataStream[]>(model.MeshCount);

	for (uint32_t i = 0; i < model.MeshCount; ++i)
	{
		uint64_t meshSize = CalculateMeshSize(model.Meshes[i]);
		meshData[i].InitStream(meshSize);

		meshData[i] = SerializeMesh(model.Meshes[i], meshSize);
		offsets[i] = meshSize;
		totalModelSize += meshSize;
	}

	DataStream modelStream(totalModelSize);
	modelStream.Write(reinterpret_cast<const uint8_t*>(&model.ModelId), sizeof(model.ModelId));
	modelStream.Write(reinterpret_cast<const uint8_t*>(&model.MeshCount), sizeof(model.MeshCount));
	modelStream.Write(reinterpret_cast<const uint8_t*>(offsets.get()), offsetDataSize);

	for (uint32_t i = 0; i < model.MeshCount; ++i)
		modelStream.Write(meshData[i].Data, offsets[i]);

	resultSize = totalModelSize;

	return modelStream;
}

Model Serialization::DesserializeModel(const uint8_t* data, const uint64_t dataSize)
{
	DataReader reader(data, dataSize);
	Model model;

	// Get model ID and mesh count
	reader.Read(reinterpret_cast<uint8_t*>(&model.ModelId), sizeof(model.ModelId));
	reader.Read(reinterpret_cast<uint8_t*>(&model.MeshCount), sizeof(model.MeshCount));

	const uint64_t offsetSize = model.MeshCount * sizeof(uint64_t);
	const std::unique_ptr<uint64_t[]> offsets = std::make_unique<uint64_t[]>(model.MeshCount);
	std::unique_ptr<Mesh[]> meshData = std::make_unique<Mesh[]>(model.MeshCount);

	// Get offset list
	reader.Read(reinterpret_cast<uint8_t*>(offsets.get()), offsetSize);

	// Get mesh data: allocate space in heap, transfer the data, interpret the data
	for (uint32_t i = 0; i < model.MeshCount; ++i)
	{
		std::unique_ptr<uint8_t[]> mesh = std::make_unique<uint8_t[]>(offsets[i]);
		reader.Read(mesh.get(), offsets[i]);
		meshData[i] = DesserializeMesh(mesh.get(), offsets[i]);
	}

	model.Meshes = std::move(meshData);

	return model;
}

uint64_t Serialization::CalculateMeshSize(const Mesh& mesh)
{
	uint64_t objectSize = sizeof(mesh.IndexCount) + sizeof(mesh.MeshId) + sizeof(mesh.VertexCount) + sizeof(mesh.Material);
	objectSize += sizeof(mesh.HorizontalWrap) + sizeof(mesh.VerticalWrap);
	objectSize += mesh.IndexCount * sizeof(mesh.IndexList.get()[0]);
	objectSize += mesh.VertexCount * sizeof(mesh.VertexList.get()[0]);

	return objectSize;
}
