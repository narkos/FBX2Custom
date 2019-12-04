#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"

struct FbxFileHeader
{
	int transformCount = 0;
	int meshCount = 0;	//Not rly needed just check vector.size
	int materialCount = 0;
	int lightCount = 0;
	int cameraCount = 0;

	char* ToRaw()
	{
		DataConverter pointer;
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