#include <string.h>
#include "CustomExporter.h"
#include "DataConverter.h"

using namespace std;

namespace CustomWriter
{
    void WriteFile(string fileName, Reader* reader)
    {
        ofstream file;
        file.open(fileName, ios::out | ios::binary);

        char* data;
        size_t dataSize;

        //int num = 5;
        //file.write(reinterpret_cast<const char *>(&num), sizeof(num));
        //data = new char[1] { (char)num };

        ConvertHeaderToRaw(reader, data, dataSize);
        if (data != NULL)
        {
            file.write(reinterpret_cast<const char *>(data), dataSize);
        }

        ConvertMeshesToRaw(reader, data, dataSize);
        if (data != NULL)
        {
            file.write(reinterpret_cast<const char *>(data), dataSize);
        }

        ConvertLightsToRaw(reader, data, dataSize);
        if (data != NULL)
        {
            file.write(reinterpret_cast<const char *>(data), dataSize);
        }

        //ConvertMaterialsToRaw(reader, data, dataSize);
        //file.write(reinterpret_cast<const char *>(data), sizeof(&data));

        ConvertCamerasToRaw(reader, data, dataSize);
        if (data != NULL)
        {
            file.write(reinterpret_cast<const char *>(data), dataSize);
        }

        file.close();



       /* ofstream myBinFile;
        myBinFile.open("example.lul", ios::out | ios::binary);
        myBinFile.write((char*)vert.name, sizeof(vert.name));
        myBinFile.write((char*)vert.position, sizeof(int) * 3);
        myBinFile.close();*/

        /*char test[100];
        ifstream myReadFile;
        myReadFile.open("example.lul", ios::binary | ios::in);
        myReadFile.read(test, sizeof(vert.name));
        cout << test << endl;
        myReadFile.read(test, sizeof(int) * 3);
        cout << (int)test[0] << (int)test[4] << (int)test[8] << endl;
        myReadFile.close();*/

        return;
    }

    void ConvertHeaderToRaw(Reader* reader, char* &data, size_t &size)
    {
        data = reader->GetHeader()->ToRaw();
        size = reader->GetHeader()->GetCurrSize();
    }

    void ConvertMeshesToRaw(Reader* reader, char* &data, size_t &size)
    {
        if (reader->GetHeader()->meshCount == 0)
        {
            data = NULL;
            size = 0;
            return;
        }

        DataConverter pointer;
        
        for (int i = 0; i < reader->GetHeader()->meshCount; i++)
        {
            pointer.Add(reader->GetMeshes()[i].ToRaw(), reader->GetMeshes()[i].GetCurrSize());
        }

        data = pointer.Get();
        size = pointer.Size();
    }

    void ConvertLightsToRaw(Reader* reader, char* &data, size_t &size)
    {
        if (reader->GetHeader()->lightCount == 0)
        {
            data = NULL;
            size = 0;
            return;
        }

        DataConverter pointer;

        for (int i = 0; i < reader->GetHeader()->lightCount; i++)
        {
            pointer.Add(reader->GetLights()[i].ToRaw(), reader->GetLights()[i].GetCurrSize());
        }

        data = pointer.Get();
        size = pointer.Size();
    }

    void ConvertMaterialsToRaw(Reader* reader, char* &data, size_t &size)
    {
        return;
    }

    void ConvertCamerasToRaw(Reader* reader, char* &data, size_t &size)
    {
        if (reader->GetHeader()->cameraCount == 0)
        {
            data = NULL;
            size = 0;
            return;
        }

        DataConverter pointer;

        for (int i = 0; i < reader->GetHeader()->cameraCount; i++)
        {
            pointer.Add(reader->GetCameras()[i].ToRaw(), reader->GetCameras()[i].GetCurrSize());
        }

        data = pointer.Get();
        size = pointer.Size();
    }
}







/*
	OLD SHIT
*/

//#include "CustomExporter.h"
//using namespace CustomFormat;
//
//void CustomExporter::Export(string fileName, Body::File *fileBody)
//{
//	// Generate H file.
//	Header::Base *fileHeader = new Header::File();
//
//	File *file = new File();
//	file.header = fileHeader;
//	file.body = fileBody;
//
//	WriteToFile(fileName, file);
//}
//
//
//void CustomExporter::WriteToFile(string fileName, File *data)
//{
//
//}


//namespace BinaryWriter {
//
//    void CreateFile() {
        //ofstream myfile;
        //myfile.open("example.txt");
        //myfile << "Writing some text. \n";
        //myfile << "Writing some more text \n";
        //myfile.close();
        //cout << "ok this works " << endl;
        ////BinaryWriter::Vertex vert ( 'test', [1, 2, 3] );

        //Vertex vert = { "TestName", { 1, 2, 3} };
        ///*strcpy(vert.name, "Testname");
        //vert.position = { 1, 2, 3 }*/;
        ////vert.name = "test";
        //cout << sizeof(vert.name) << endl;
        //cout << sizeof(vert.position) << endl;
        //cout << sizeof(vert) << endl;

        //ofstream myBinFile;
        //myBinFile.open("example.lul", ios::out | ios::binary);
        //myBinFile.write((char*)vert.name, sizeof(vert.name));
        //myBinFile.write((char*)vert.position, sizeof(int) * 3);
        //myBinFile.close();

        //char test[100];
        //ifstream myReadFile;
        //myReadFile.open("example.lul", ios::binary | ios::in);
        //myReadFile.read(test, sizeof(vert.name));
        //cout << test << endl;
        //myReadFile.read(test, sizeof(int)* 3);
        //cout << (int)test[0] << (int)test[4] << (int)test[8] << endl;
        //myReadFile.close();
        //return;
//    }
//}

//
//namespace AsciiWriter {
//	void CreateFile(Reader* readData)
//	{
//		FbxFileHeader* globalHeader = readData->GetHeader();
//		cout << globalHeader->meshCount;
//
//		
//		ofstream asciiFile;
//		asciiFile.open("Exports/asciiExport.txt");
//		asciiFile << "Meshes: " << globalHeader->meshCount << endl;
//		asciiFile << "Lights: " << globalHeader->lightCount << endl;
//		asciiFile << "Materials: " << globalHeader->materialCount << endl;
//		asciiFile << "Cameras: " << globalHeader->cameraCount << endl;
//
//		vector<Mesh> meshes = readData->GetMeshes();
//		for (int i = 0; i < meshes.size(); i++)
//		{
//			asciiFile << endl;
//			AsciiWriter::WriteMesh(&meshes.at(i), &asciiFile);
//
//		}
//
//		asciiFile.close();
//	}
//
//	void WriteMesh(Mesh* mesh, ofstream* file)
//	{
//		*file << AsciiWriter::WriteMeshHeader(mesh).c_str();
//	}
//
//	string WriteMeshHeader(Mesh* mesh)
//	{
//		string outstring;
//		outstring = WriteTransform(mesh->meshTransform);
//		outstring += "Vertex Count: " + to_string(mesh->meshHeader.vertexCount) + "\n";
//		outstring += "Triangle Count: " + to_string(mesh->meshHeader.triangleCount) + "\n";
//		outstring += "UV Count: " + to_string(mesh->meshHeader.uvCount) + "\n";
//		outstring += "Face Index Count: " + to_string(mesh->meshHeader.faceIndexCount) + "\n";
//
//		outstring += "Vertices:\n";
//
//		for (unsigned int i = 0; i < mesh->meshHeader.vertexCount; i++)
//		{
//			outstring 
//				+= to_string(mesh->controlPoints.at(i)[0]) + " " 
//				+ to_string(mesh->controlPoints.at(i)[1]) + " "
//				+ to_string(mesh->controlPoints.at(i)[2]) + " "
//				+ to_string(mesh->controlPoints.at(i)[3]) + " \n";
//		}
//		cout << outstring.c_str() << endl;
//		//cout << to_string(mesh->meshIndices.size()) << "  " << to_string(mesh->meshHeader.faceIndexCount) << endl << endl;
//		int counter = 0;
//		for (unsigned int i = 0; i < mesh->meshHeader.faceIndexCount; i++)
//		{
//			switch (counter)
//			{
//			case 0:
//				outstring += "[ " + to_string(mesh->meshIndices.at(i)) + " , ";
//				break;
//			case 1:
//				outstring += to_string(mesh->meshIndices.at(i)) + " , ";
//				break;
//			case 2:
//				outstring += to_string(mesh->meshIndices.at(i)) + " ]\n";
//				counter = -1;
//				break;
//			}
//			counter++;
//			//cout << endl << mesh->meshIndices.at(i);
//		}
//		cout << outstring.c_str() << endl;
//		
//		return outstring;
//	}
//
//	string WriteFbxDoubleVector(FbxDouble3 vec)
//	{
//		string outstring;
//		outstring = to_string(vec[0]) + ", " + to_string(vec[1]) + ", " + to_string(vec[2]);
//		return outstring;
//	}
//
//	string WriteTransform(Transform transform)
//	{
//		string outstring;
//		outstring = "Transform: " + transform.name + "\n";
//		outstring += "Position: " + WriteFbxDoubleVector(transform.position) + "\n";
//		outstring += "Scale: " + WriteFbxDoubleVector(transform.scale) + "\n";
//		outstring += "Rotation: " + WriteFbxDoubleVector(transform.rotation) + "\n";
//		return outstring;
//	}
//}