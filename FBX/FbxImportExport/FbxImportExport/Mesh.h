#pragma once
#include <fbxsdk.h>

#include "Vertex.h"
#include "DataConverter.h"

struct MeshHeader
{
    DataConverter pointer;

	string transformName;
	int vertexCount;


	//int normalCount; ??
	int controlPointCount;
	int uvCount;
	int faceIndexCount; //Vertex count * 3
	int triangleCount;

public:
	size_t GetCurrSize()
	{
        ToRaw(); // Just so that the size exists.
		return pointer.Size();
	}

	char* ToRaw()
	{
        pointer.Clear();
		pointer.Add(transformName);
		pointer.Add(vertexCount);

		char* raw = pointer.Get();
		int size = pointer.Size();
		return raw;
	}
};

struct Mesh
{
    DataConverter pointer;

	MeshHeader meshHeader;
	vector<Vertex> meshVertices; //Original

	//Vertex* meshVertexList;
	vector<array<float, 4>> controlPoints;
	vector<int> meshIndices;

public:
	size_t GetCurrSize()
	{
        ToRaw(); // Just so that the size exists.
		return pointer.Size();
	}

	char* ToRaw()
	{
        pointer.Clear();
		pointer.Add(meshHeader.ToRaw(), meshHeader.GetCurrSize());

		for (int i = 0; i < meshHeader.vertexCount; i++)
		{
			pointer.Add(meshVertices[i].ToRaw(), meshVertices[i].GetCurrSize());
		}

		char* raw = pointer.Get();
		int size = pointer.Size();
		return raw;
	}
};