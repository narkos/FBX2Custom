#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <array>
#include <vector>
#include "Assets.h"
//#include <DirectXMath.h>	//Neccessary if use of XMFLOATS etc. 
//#include <d3d11.h>

using namespace std;
//using namespace DirectX;	//Neccessary if use of XMFLOATS etc.

//Traversing the scene!: https://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_29C09995_47A9_4B49_9535_2F6BDC5C4107_htm

class Reader
{
private:
	//Data to store for production of custom file:
	FbxFileHeader globalHeader;	//Global File Header

    vector<Transform> transforms;
	vector<Mesh> meshes; //Fill this with the dada
	vector<Camera> cameras;
	vector<Light> lights;

public:
	Reader();
	~Reader();

	bool ImportFBX();
	bool ReadSceneData(FbxScene* scene);

	FbxFileHeader* GetHeader();
	vector<Mesh>& GetMeshes();
    vector<Light>& GetLights();
    vector<Camera>& GetCameras();
    vector<Transform>& GetTransforms();

	void ProcessMesh(FbxNode* currentNode, FbxScene* scene, Mesh& omesh, Transform& otransform);
    void ProcessCamera(FbxNode* currentNode, Camera& ocamera, Transform& otransform);
    void ProcessLight(FbxNode* currentNode, Light& olight, Transform& otransform);

	FbxDouble3 ProcessNormals(FbxMesh* mesh, int index, int vertexNr);
	FbxDouble3 ProcessTangents(FbxMesh* mesh, int index, int vertexNr);
	Transform GetNodeTransform(FbxNode* currentNode);
	void PrintNodeGeneralData(FbxNode* pNode);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintTestData();
	void PrintFbxMatrix(FbxMatrix inMatrix);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
};