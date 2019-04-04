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
	int meshCount = 0;
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
	int normalCount;
	int uvCount;
	int faceIndexCount; //Vertex count * 3
};

struct Transform
{
	FbxDouble3 position;
	FbxDouble3 scale;
	FbxDouble3 rotation;
};

struct Mesh
{
	MeshHeader meshHeader;
	Vertex* meshVertexList;
	Transform meshTransform; 
};

class Importer
{
private:
	//Data to store for production of custom file:

	FbxFileHeader globalHeader;	//Global File Header

	const char* nodeName;
	int nodeType; //1.Mesh - 2:Camera - 3:Light (For example)

	Transform nodeTransformData;
	Mesh meshData; //Fill this with the dada


public:
	Importer();
	~Importer();

	bool ImportFBX();
	bool ReadSceneData(FbxScene* scene);
	void ProcessMesh(FbxNode* currentNode);
	void ProcessNormals(FbxMesh* mesh, int index, int vertexNr);
	void PrintNodeGeneralData(FbxNode* pNode);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	FbxString Importer::GetAttributeTypeName(FbxNodeAttribute::EType type);

};