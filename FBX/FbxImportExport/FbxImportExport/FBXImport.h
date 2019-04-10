#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
//#include <DirectXMath.h>	//Neccessary if use of XMFLOATS etc. 
//#include <d3d11.h>

using namespace std;
//using namespace DirectX;	//Neccessary if use of XMFLOATS etc.

//Traversing the scene!: https://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_29C09995_47A9_4B49_9535_2F6BDC5C4107_htm

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

class Reader
{
private:
	//Data to store for production of custom file:
	FbxFileHeader globalHeader;	//Global File Header

	vector<Mesh> meshes; //Fill this with the dada
	vector<Camera> cameras;
	vector<Light> lights;

public:
	Reader();
	~Reader();

	bool ImportFBX();
	bool ReadSceneData(FbxScene* scene);
	Mesh ProcessMesh(FbxNode* currentNode, FbxScene* scene);
	FbxDouble3 ProcessNormals(FbxMesh* mesh, int index, int vertexNr);
	FbxDouble3 ProcessTangents(FbxMesh* mesh, int index, int vertexNr);
	Camera ProcessCamera(FbxNode* currentNode);
	Light ProcessLight(FbxNode* currentNode);
	Transform GetNodeTransform(FbxNode* currentNode);
	void PrintNodeGeneralData(FbxNode* pNode);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintTestData();
	void PrintFbxMatrix(FbxMatrix inMatrix);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
};