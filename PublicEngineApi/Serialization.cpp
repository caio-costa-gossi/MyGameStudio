#include "Serialization.h"

#include "DataReader.h"
#include "../MyGameStudio/DataStream.h"

DataStream Serialization::SerializeMesh(const Mesh& mesh, uint64_t& resultSize)
{
	uint64_t objectSize = sizeof(mesh.TextureAssetId) + sizeof(mesh.IndexCount) + sizeof(mesh.MeshId) + sizeof(mesh.VertexCount);
	objectSize += sizeof(mesh.HorizontalWrap) + sizeof(mesh.VerticalWrap);
	objectSize += mesh.IndexCount * sizeof(mesh.IndexList.get()[0]);
	objectSize += mesh.VertexCount * sizeof(mesh.VertexList.get()[0]);

	resultSize = objectSize;
	DataStream stream(objectSize);

	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.TextureAssetId), sizeof(mesh.TextureAssetId));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.IndexCount), sizeof(mesh.IndexCount));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.MeshId), sizeof(mesh.MeshId));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.VertexCount), sizeof(mesh.VertexCount));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.HorizontalWrap), sizeof(mesh.HorizontalWrap));
	stream.Write(reinterpret_cast<const uint8_t*>(&mesh.VerticalWrap), sizeof(mesh.VerticalWrap));

	stream.Write(reinterpret_cast<const uint8_t*>(mesh.IndexList.get()), mesh.IndexCount * sizeof(mesh.IndexList.get()[0]));
	stream.Write(reinterpret_cast<const uint8_t*>(mesh.VertexList.get()), mesh.VertexCount * sizeof(mesh.VertexList.get()[0]));

	return stream;
}

Mesh Serialization::DesserializeMesh(const uint8_t* data, const uint64_t dataSize)
{
	DataReader reader(data, dataSize);
	Mesh mesh = { };

	reader.Read(reinterpret_cast<uint8_t*>(&mesh.TextureAssetId), sizeof(mesh.TextureAssetId));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.IndexCount), sizeof(mesh.IndexCount));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.MeshId), sizeof(mesh.MeshId));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.VertexCount), sizeof(mesh.VertexCount));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.HorizontalWrap), sizeof(mesh.HorizontalWrap));
	reader.Read(reinterpret_cast<uint8_t*>(&mesh.VerticalWrap), sizeof(mesh.VerticalWrap));

	mesh.IndexList = std::make_unique<uint32_t[]>(mesh.IndexCount);
	mesh.VertexList = std::make_unique<Vertex[]>(mesh.VertexCount);

	reader.Read(reinterpret_cast<uint8_t*>(mesh.IndexList.get()), mesh.IndexCount * sizeof(mesh.IndexList.get()[0]));
	reader.Read(reinterpret_cast<uint8_t*>(mesh.VertexList.get()), mesh.VertexCount * sizeof(mesh.VertexList.get()[0]));

	return mesh;
}
