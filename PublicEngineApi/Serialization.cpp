#include "Serialization.h"

#include "../MyGameStudio/DataStream.h"

DataStream Serialization::SerializeMesh(const Mesh& mesh)
{
	uint64_t objectSize = sizeof(mesh.TextureAssetId) + sizeof(mesh.IndexCount) + sizeof(mesh.MeshId) + sizeof(mesh.VertexCount);
	objectSize += sizeof(mesh.HorizontalWrap) + sizeof(mesh.VerticalWrap);
	objectSize += mesh.IndexCount * sizeof(mesh.IndexList.get()[0]);
	objectSize += mesh.VertexCount * sizeof(mesh.VertexList.get()[0]);

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

Mesh Serialization::DesserializeMesh(uint8_t* data)
{
	return { };
}
