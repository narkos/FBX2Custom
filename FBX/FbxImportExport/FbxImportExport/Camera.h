#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"


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
		DataConverter pointer;

		pointer.Add(transformName);
		pointer.Add(viewDirection);
		pointer.Add(upVector);
		pointer.Add(isOrtho);

		char* raw = pointer.Get();
		int size = pointer.Size();
		return raw;
	}
};