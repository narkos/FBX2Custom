#pragma once
#ifndef ASSETS_H
#include <fbxsdk.h>
#include <vector>
#include <string>

using namespace std;

class CharPointer {
private:
    int addCount = 0;
    //char* currPointer = NULL;
    vector<char*> pointers;
    size_t currSize;
public:
    CharPointer()
    {
        addCount = 0;
        //currPointer = NULL;
    }

    template <class T>
    void Add(T data)
    {
        size_t size = sizeof(data);
        char* newData = reinterpret_cast<char *>(&data);
        
        if (this->addCount == 0)
        {
            currSize = size;
        }
        else 
        {
            currSize = size + currSize;
        }

        //currPointer = new char[size];
        
        //strcpy(currPointer, newData, size);

        char* tmpPointer = new char[size];
        memcpy(tmpPointer, newData, size);

        pointers.push_back(tmpPointer);

        //if (this->addCount == 0)
        //{

        //}
        //else 
        //{
        //    size_t prevSize = currSize;
        //    char* prevData = currPointer;
        //    currSize = currSize + size;
        //    currPointer = new char[currSize];

        //    strncpy(currPointer, prevData, prevSize);
        //    strncpy(currPointer + prevSize, newData, size);
        //    //strcat(currPointer, newData);

        //    // TODO: Release prev data.
        //}

        this->addCount++;
    }

    char* Get()
    {
        char* allPointers = new char[currSize];

        if (pointers.size() != 0)
        {        
            // TODO FIX MEMCPY N SHIET
            memcpy(allPointers, pointers[0], 5);

            for (int i = 1; i < pointers.size(); i++)
            {
                memcpy(allPointers, pointers[i], 5);
            }

            return allPointers;
        }
        else
        {
            return NULL;
        }
    }

    int Size()
    {
        return currSize;
    }
};

struct FbxFileHeader
{
    int transformCount = 0;
	int meshCount = 0;	//Not rly needed just check vector.size
	int materialCount = 0;
	int lightCount = 0;
	int cameraCount = 0;

    char* ToRaw()
    {
        CharPointer pointer;
        pointer.Add(transformCount);
        pointer.Add(meshCount);
        pointer.Add(materialCount);
        pointer.Add(lightCount);
        pointer.Add(cameraCount);

        char* raw = pointer.Get();
        int size = pointer.Size();
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