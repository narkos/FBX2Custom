#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <fbxsdk.h>

#include "Assets.h"
#include "FBXImport.h"

namespace CustomWriter
{
    void WriteFile(string fileName, Reader* reader);
    void ConvertHeaderToRaw(Reader* reader, char* &data, size_t &size);
    void ConvertMeshesToRaw(Reader* reader, char* &data, size_t &size);
    void ConvertLightsToRaw(Reader* reader, char* &data, size_t &size);
    void ConvertMaterialsToRaw(Reader* reader, char* &data, size_t &size);
    void ConvertCamerasToRaw(Reader* reader, char* &data, size_t &size);
}

//namespace BinaryWriter {
//	void CreateFile();
//	struct Vertex 
//	{
//		char name[50];
//		int position[3];
//		/*Vertex(char* p_name, int p_pos[3])
//		{
//			name = p_name;
//			position = p_pos;
//		}*/
//	};
//}
//
//namespace AsciiWriter {
//	void CreateFile(Reader* readData);
//	void WriteMesh(Mesh* mesh, ofstream* file);
//	string WriteMeshHeader(Mesh* mesh);
//	string WriteFbxDoubleVector(FbxDouble3 vec);
//	string WriteTransform(Transform transform);
//}