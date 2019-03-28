//#pragma once
//#include "CustomFormat.h"
//#include <iostream>
//#include <fstream>
//using namespace std;
//
//namespace CustomFormat
//{
//	class CustomExporter
//	{
//	public:
//		void Export(string fileName, Body::File *fileBody);
//
//		void WriteToFile(string fileName, File *data);
//	};
//}
namespace BinaryWriter {
	void CreateFile();
	struct Vertex 
	{
		char name[50];
		int position[3];
		/*Vertex(char* p_name, int p_pos[3])
		{
			name = p_name;
			position = p_pos;
		}*/
	};
}