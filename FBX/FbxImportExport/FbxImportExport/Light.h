#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"

struct Light
{
    DataConverter pointer;

	string transformName;
	FbxDouble3 color;
	FbxDouble intensity;

public:
    size_t GetCurrSize()
    {
        ToRaw(); // Just so that the size exists.
        return pointer.Size();
    }

    char* ToRaw()
    {
        pointer.Clear();
        pointer.Add(transformName);
        pointer.Add(color);
        pointer.Add(intensity);

        char* raw = pointer.Get();
        int size = pointer.Size();
        return raw;
    }
};