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

    char* ToRaw()
    {
        char* raw = new char[sizeof(FbxFileHeader)];
        strcpy(raw, to_string(meshCount).c_str());
        strcat(raw, to_string(materialCount).c_str());
        strcat(raw, to_string(lightCount).c_str());
        strcat(raw, to_string(cameraCount).c_str());
        return raw;
    }
};

struct Vertex
{
	FbxDouble3 vPos;
	FbxDouble2 vUV;
	FbxDouble3 vNormal;
	FbxDouble3 vTangent;
	FbxDouble3 vBiTangent;

public:
    char* ToRaw()
    {
        char* raw = new char[sizeof(Vertex)];

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(vPos[i]).c_str());
        }

        for (int i = 0; i < 2; i++)
        {
            strcat(raw, to_string(vUV[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(vNormal[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(vTangent[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(vBiTangent[i]).c_str());
        }

        return raw;
    }
};

struct Transform
{
	string name;
	FbxDouble3 position;
	FbxDouble3 scale;
	FbxDouble3 rotation;

public:
    char* ToRaw()
    {
        char* raw = new char[sizeof(Transform)];
        strcpy(raw, name.c_str());

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(position[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(scale[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            strcat(raw, to_string(rotation[i]).c_str());
        }

        return raw;
    }
};

struct MeshHeader
{
    string transformName;
	int vertexCount;
	//int normalCount; ??
	int uvCount;
	int faceIndexCount; //Vertex count * 3
	int triangleCount;

public:
    char* ToRaw()
    {
        char* raw = new char[sizeof(MeshHeader)];
        strcpy(raw, transformName.c_str());
        strcat(raw, to_string(vertexCount).c_str());
        strcat(raw, to_string(uvCount).c_str());
        strcat(raw, to_string(faceIndexCount).c_str());
        strcat(raw, to_string(triangleCount).c_str());
        return raw;
    }
};

struct Mesh
{
	MeshHeader meshHeader;
	//Vertex* meshVertexList;
	vector<array<float, 4>> controlPoints;
	vector<Vertex> meshVertices; //Original
	vector<Vertex> meshVertexListNoIndex; //New Because of Simon - ACTUALLY HAS INDICES OMEGALUL
	vector<int> meshIndices;

public:
    char* ToRaw()
    {
        char* raw;
        char* header = meshHeader.ToRaw();
        char* body;

        int rawSize = strlen(header) + strlen(body);
        raw = new char[rawSize];

        strcpy(raw, header);
        strcat(raw, body);

        return raw;
    }
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

    char* ToRaw()
    {
        char* raw = new char[sizeof(MeshHeader)];
        //strcpy(raw, transformName.c_str());
        //strcat(raw, to_string(vertexCount).c_str());
        //strcat(raw, to_string(uvCount).c_str());
        //strcat(raw, to_string(faceIndexCount).c_str());
        //strcat(raw, to_string(triangleCount).c_str());
        return raw;
    }
};

struct Light
{
	Transform lightTransform;
	FbxDouble3 color;
	FbxDouble intensity;

    char* ToRaw()
    {
        char* raw = new char[sizeof(MeshHeader)];
        //strcpy(raw, transformName.c_str());
        //strcat(raw, to_string(vertexCount).c_str());
        //strcat(raw, to_string(uvCount).c_str());
        //strcat(raw, to_string(faceIndexCount).c_str());
        //strcat(raw, to_string(triangleCount).c_str());
        return raw;
    }
};


#endif