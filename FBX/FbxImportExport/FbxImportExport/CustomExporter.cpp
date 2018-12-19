#include "CustomExporter.h"
using namespace CustomFormat;

void CustomExporter::Export(string fileName, Body::File *fileBody)
{
	// Generate H file.
	Header::Base *fileHeader = new Header::File();

	File *file = new File();
	file.header = fileHeader;
	file.body = fileBody;

	WriteToFile(fileName, file);
}


void CustomExporter::WriteToFile(string fileName, File *data)
{

}