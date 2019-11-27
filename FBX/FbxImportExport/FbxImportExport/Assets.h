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
    vector<size_t> pointerSizes;

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

        char* tmpPointer = new char[size];
        memcpy(tmpPointer, newData, size);

        pointers.push_back(tmpPointer);
        pointerSizes.push_back(size);

        this->addCount++;
    }

    void Add(string data)
    {
        size_t size = sizeof(char) * data.length();
        char* newData = new char[size];

        if (this->addCount == 0)
        {
            currSize = size;
        }
        else
        {
            currSize = size + currSize;
        }

        memcpy(newData, data.c_str(), size);
        pointers.push_back(newData);
        pointerSizes.push_back(size);

        this->addCount++;
    }

    void Add(char* dataPointer, size_t dataSize)
    {
        if (this->addCount == 0)
        {
            currSize = dataSize;
        }
        else
        {
            currSize = dataSize + currSize;
        }

        pointers.push_back(dataPointer);
        pointerSizes.push_back(dataSize);

        this->addCount++;
    }

    char* Get()
    {
        char* allPointers = new char[currSize];

        if (pointers.size() != 0)
        {        
            // TODO FIX MEMCPY N SHIET
            memcpy(allPointers, pointers[0], pointerSizes[0]);

            size_t accumOffset = pointerSizes[0];
            for (int i = 1; i < pointers.size(); i++)
            {
                memcpy(allPointers + accumOffset, pointers[i], pointerSizes[i]);
                accumOffset += pointerSizes[i];
            }

            return allPointers;
        }
        else
        {
            return NULL;
        }
    }

    size_t Size()
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

    char* ToRaw()
    {
        CharPointer pointer;

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(vPos[i]);
        }

        for (int i = 0; i < 2; i++)
        {
            pointer.Add(vUV[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(vNormal[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(vTangent[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(vBiTangent[i]);
        }

        char* raw = pointer.Get();
        int size = pointer.Size();
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
    size_t GetSize()
    {
        return (name.length() * sizeof(char)) + (sizeof(FbxDouble3) * 3);
    }

    char* ToRaw()
    {
        CharPointer pointer;

        pointer.Add(name);

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(position[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(scale[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            pointer.Add(rotation[i]);
        }

        char* raw = pointer.Get();
        int size = pointer.Size();
        return raw;
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
    size_t GetSize()
    {
        return transformName.length() * sizeof(char) + sizeof(int);
    }

    char* ToRaw()
    {
        CharPointer pointer;

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
        CharPointer pointer;

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
    size_t GetSize()
    {
        return (transformName.length() * sizeof(char)) + (sizeof(FbxDouble3) * 2) + sizeof(bool);
    }

    char* ToRaw()
    {
        CharPointer pointer;

        pointer.Add(transformName);
        pointer.Add(viewDirection);
        pointer.Add(upVector);
        pointer.Add(isOrtho);

        char* raw = pointer.Get();
        int size = pointer.Size();
        return raw;
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