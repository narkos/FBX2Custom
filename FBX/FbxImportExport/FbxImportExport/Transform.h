#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"

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
		DataConverter pointer;

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