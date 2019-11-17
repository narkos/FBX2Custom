#pragma once
#ifndef ASSETS_H
#include <fbxsdk.h>
#include <vector>
#include <string>

using namespace std;

struct FbxFileHeader
{
    int transformCount = 0;
	int meshCount = 0;	//Not rly needed just check vector.size
	int materialCount = 0;
	int lightCount = 0;
	int cameraCount = 0;

    string ToRaw()
    {
        string rawString = "";
        rawString += to_string(transformCount);
        rawString += to_string(meshCount);
        rawString += to_string(materialCount);
        rawString += to_string(lightCount);
        rawString += to_string(cameraCount);
        return rawString;
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
    string ToRaw()
    {
        string rawString = "";

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(vPos[i]);
        }

        for (int i = 0; i < 2; i++)
        {
            rawString += to_string(vUV[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(vNormal[i]).c_str();
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(vTangent[i]).c_str();
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(vBiTangent[i]).c_str();
        }

        return rawString;
    }
};

struct Transform
{
	string name;
	FbxDouble3 position;
	FbxDouble3 scale;
	FbxDouble3 rotation;

public:
    string ToRaw()
    {
        string rawString = "";
        rawString += name;

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(position[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(scale[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(rotation[i]);
        }

        return rawString;
    }
};

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
    string ToRaw()
    {
        string rawString = "";
        rawString += transformName;
        rawString += to_string(vertexCount);
        return rawString;
    }
};

struct Mesh
{
	MeshHeader meshHeader;
	//Vertex* meshVertexList;
	vector<array<float, 4>> controlPoints;
	vector<Vertex> meshVertices; //Original
	vector<int> meshIndices;

public:
    string ToRaw()
    {
        string rawString = "";
        rawString += meshHeader.ToRaw();

        for (int i = 0; i < meshHeader.vertexCount; i++)
        {
            rawString += meshVertices[i].ToRaw();
        }

        return rawString;
    }
};

struct Camera
{
    string transformName;
	FbxDouble3 viewDirection;	//AKA LookAt
	FbxDouble3 upVector;
	//FbxDouble3 right;	//Can't find property in FBX
	bool isOrtho;


	FbxMatrix projectionMatrix;
	//double rot[4]; //Quaternion rotation of the camera (x, y, z, w)

public:
    string ToRaw()
    {
        string rawString = "";
        rawString += transformName;

        for (int i = 0; i < 3; i++)
        {
            rawString += viewDirection[i];
        }

        for (int i = 0; i < 3; i++)
        {
            rawString += upVector[i];
        }

        rawString += to_string(isOrtho);

        return rawString;
    }
};

struct Light
{
    string transformName;
	FbxDouble3 color;
	FbxDouble intensity;

public:
    string ToRaw()
    {
        string rawString = "";
        rawString += transformName;

        for (int i = 0; i < 3; i++)
        {
            rawString += to_string(color[i]);
        }

        rawString += to_string(intensity).c_str();

        return rawString;
    }
};


#endif