#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#include <DirectXMath.h>
//#include <d3d11.h>

using namespace std;
using namespace DirectX;

//Traversing the scene!: https://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_29C09995_47A9_4B49_9535_2F6BDC5C4107_htm

class Importer
{
private:
	//Data to store for production of custom file:

	int objectCount;
	int meshCount;
	int cameraCount;


	struct Vertex
	{
		XMFLOAT3 vPos;
		XMFLOAT2 vUV;
		XMFLOAT3 vNormal;
	};


public:
	Importer();
	~Importer();

	bool ImportFBX();
	bool ReadSceneData(FbxScene* scene);
	void PrintNode(FbxNode* pNode);


	void PrintStuffs();
};