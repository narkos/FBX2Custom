#pragma once

#include "Assets.h"


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

namespace AsciiWriter {
	void CreateFile(FbxFileHeader* header);
}