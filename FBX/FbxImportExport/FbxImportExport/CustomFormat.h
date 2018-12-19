#include <fbxsdk.h>
#include <iostream>
#include "Utility.h"
#pragma comment(lib, "libfbxsdk-md.lib")
using namespace std;
#pragma once

namespace CustomFormat 
{
	class BaseObject 
	{
	public:
		Header *header;
		Body *body;
	};

	// Headers.
#pragma region ---HEADERS---
	namespace Header
	{
		class Header
		{

		};

		// Mesh
		class MeshHeader : Header
		{
		public:
			int nrVertices;
			int nrFaceIndices;
			int materialIndex;
			int nrMorph;
		};

		// Material
		class MaterialHeader : Header
		{
		public:
			int materialIndex;
			Color color;
			string textureName;
		};

		class LightHeader
		{

		};
	}


#pragma endregion ---HEADERS---

	// Bodies.
#pragma region ---BODY---
	class Body
	{

	};
#pragma endregion ---BODY---
}