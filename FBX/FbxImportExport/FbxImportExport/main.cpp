//#include <iostream>
#include "FBXImport.h"

//#inlcude <fbxfilesdk/fbxio/fbxiosettings.h>

#pragma comment(lib, "libfbxsdk-md.lib")
using namespace std;

int main() {
	
	//Moved to object 
	//Importer* importedFBX = new Importer();
	//importedFBX->importFBX();

	int i;

	Reader* importer = new Reader();

	//importer->PrintStuffs(); //TEST
	
	importer->ImportFBX(); //Now comes with self destruct b*tches

	importer->PrintTestData();


	//FBXSDK_printf("\nTest Where does this go?\n"); //Also TEST
	
	getchar();
	return 0;
}