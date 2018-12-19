#include <fbxsdk.h>
#include <iostream>
#include "Utility.h"
#pragma comment(lib, "libfbxsdk-md.lib")
using namespace std;
#pragma once

namespace CustomFormat 
{
	class File
	{
	public:
		Header::Base *header;
		Body::Base *body;
	};

	// Headers.
#pragma region ---HEADERS---
	namespace Header
	{
		class Base
		{

		};

		// File
		class File : public Base
		{
		public:
			int nrMeshes;
			int nrMaterials;
			int nrLights;
			int nrMorphs;
		};

		// Mesh
		class Mesh : public Base
		{
		public:
			int nrVertices;
			int nrFaceIndices;
			int materialIndex;
			int nrMorph;
		};

		// Material
		class Material : public Base
		{
		public:
			int materialIndex;
			Color color;
			string textureName;
		};

		// Light
		class Light : public Base
		{
		public:
			Vector3 position;
			float range;
		};

		// Camera
		class Camera : public Base
		{
		public:
			Vector3 position;
			Vector3 rotation;
		};
	}
#pragma endregion ---HEADERS---

	// Bodies.
#pragma region ---BODY---
	namespace Body
	{
		class Base
		{

		};

		class File : public Base
		{
			Mesh *meshes;
			Header::Material *materials;
			Header::Light *lights;
			Header::Camera camera;
		};

		class Mesh : public Base
		{
			Vertex *vertices;
			int *indices;
		};

		class Vertex : public Base
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;
			Vector3 tangent;
		};
	}
#pragma endregion ---BODY---
}