#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"

struct Vertex
{
    DataConverter pointer;

	FbxDouble3 vPos;
	FbxDouble2 vUV;
	FbxDouble3 vNormal;
	FbxDouble3 vTangent;
	FbxDouble3 vBiTangent;

public:
    size_t GetCurrSize()
    {
        ToRaw(); // Just so that the size exists.
        return pointer.Size();
    }

	char* ToRaw()
	{
        pointer.Clear();
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
