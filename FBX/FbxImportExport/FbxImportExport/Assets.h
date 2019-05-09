#pragma once
#ifndef ASSETS_H
#include <fbxsdk.h>
#include <vector>

using namespace std;

struct FbxFileHeader
{
	int meshCount = 0;	//Not rly needed just check vector.size
	int materialCount = 0;
	int lightCount = 0;
	int cameraCount = 0;
};

struct Vertex
{
	FbxDouble3 vPos;
	FbxDouble2 vUV;
	FbxDouble3 vNormal;
	FbxDouble3 vTangent;
	FbxDouble3 vBiTangent;
};

struct MeshHeader
{
	int vertexCount;
	//int normalCount; ??
	int uvCount;
	int faceIndexCount; //Vertex count * 3
	vector<int> meshIndices; //Prolly move
};

struct Transform
{
	const char* name;
	FbxDouble3 position;
	FbxDouble3 scale;
	FbxDouble3 rotation;
};

struct Mesh
{
	Transform meshTransform; //Be here or in the mesh header?
	MeshHeader meshHeader;
	//Vertex* meshVertexList;
	vector<Vertex> meshVertices; //Original
	vector<Vertex> meshVertexListNoIndex; //New Because of Simon - ACTUALLY HAS INDICES OMEGALUL
};

struct Camera
{
public:
	Transform cameraTransform;
	FbxDouble3 viewDirection;	//AKA LookAt
	FbxDouble3 upVector;
	//FbxDouble3 right;	//Can't find property in FBX
	bool isOrtho;
	FbxMatrix projectionMatrix;
	//double rot[4]; //Quaternion rotation of the camera (x, y, z, w)
};

struct Light
{
	Transform lightTransform;
	FbxDouble3 color;
	FbxDouble intensity;
};


#endif