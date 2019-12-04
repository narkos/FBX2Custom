#pragma once
#include <fbxsdk.h>

#include "Vertex.h"
#include "DataConverter.h"

struct MeshHeader
{
	string transformName;
	int vertexCount;


	//int normalCount; ??
	int controlPointCount;
	int uvCount;
	int faceIndexCount; //Vertex count * 3
	int triangleCount;

public:
	size_t GetSize()
	{
		return transformName.length() * sizeof(char) + sizeof(int);
	}

	char* ToRaw()
	{
		DataConverter pointer;

		pointer.Add(transformName);
		pointer.Add(vertexCount);

		char* raw = pointer.Get();
		int size = pointer.Size();
		return raw;
	}
};

struct Mesh
{
	MeshHeader meshHeader;
	vector<Vertex> meshVertices; //Original

	//Vertex* meshVertexList;
	vector<array<float, 4>> controlPoints;
	vector<int> meshIndices;

public:
	size_t GetSize()
	{
		return meshHeader.GetSize() + (meshHeader.vertexCount * sizeof(Vertex));
	}

	char* ToRaw()
	{
		DataConverter pointer;

		pointer.Add(meshHeader.ToRaw(), meshHeader.GetSize());

		for (int i = 0; i < meshHeader.vertexCount; i++)
		{
			pointer.Add(meshVertices[i].ToRaw(), sizeof(Vertex));
		}

		char* raw = pointer.Get();
		int size = pointer.Size();
		return raw;
	}
};