#pragma once
#include <fbxsdk.h>

#include "DataConverter.h"

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