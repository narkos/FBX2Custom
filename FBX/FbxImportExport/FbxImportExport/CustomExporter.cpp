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

#include <iostream>
#include <fstream>
#include <string.h>
#include "CustomExporter.h"

using namespace std;


namespace BinaryWriter {
	
	void CreateFile() {
		ofstream myfile;
		myfile.open("example.txt");
		myfile << "Writing some text. \n";
		myfile << "Writing some more text \n";
		myfile.close();
		cout << "ok this works " << endl;
		//BinaryWriter::Vertex vert ( 'test', [1, 2, 3] );

		Vertex vert = { "TestName", { 1, 2, 3} };
		/*strcpy(vert.name, "Testname");
		vert.position = { 1, 2, 3 }*/;
		//vert.name = "test";
		cout << sizeof(vert.name) << endl;
		cout << sizeof(vert.position) << endl;
		cout << sizeof(vert) << endl;

		ofstream myBinFile;
		myBinFile.open("example.lul", ios::out | ios::binary);
		myBinFile.write((char*)vert.name, sizeof(vert.name));
		myBinFile.write((char*)vert.position, sizeof(int) * 3);
		myBinFile.close();

		char test[100];
		ifstream myReadFile;
		myReadFile.open("example.lul", ios::binary | ios::in);
		myReadFile.read(test, sizeof(vert.name));
		cout << test << endl;
		myReadFile.read(test, sizeof(int)* 3);
		cout << (int)test[0] << (int)test[4] << (int)test[8] << endl;
		myReadFile.close();
		return;
	}
}