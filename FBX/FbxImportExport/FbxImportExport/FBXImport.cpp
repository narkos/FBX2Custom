#include "FBXImport.h"

using namespace std;

Importer::Importer()
{
	//Initialize Here
}

Importer::~Importer()
{
	//Destroy Here

}

void Importer::PrintStuffs()
{
	cout << "\nIS This real life?\n";

}

bool Importer::ImportFBX()
{
	//Create the FBX SDK Manager
	FbxManager* lSdkManager = FbxManager::Create();

	//Create the object responsible for Import/Export settings
	FbxIOSettings* ImpExpSettings = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ImpExpSettings);

	// https://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_50489A8A_457C_4B54_80E1_5572A16F7F17_htm
	// Configure the Import/Export settings
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, false);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, false);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, false);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(lSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);


	//Create the importer
	FbxImporter* fbxImporter = FbxImporter::Create(lSdkManager, "");


	//Define file name. Who needs to be dynamic yo
	const char* fbxFilename = "Deer_LowPoly.fbx";

	//Initialize the damn importher. 
	bool lImportStatus = fbxImporter->Initialize(fbxFilename, -1, lSdkManager->GetIOSettings());

	if (!lImportStatus)
	{
		cout << "\nSHIT, Error whilst initializing the importer:\n";
		cout << fbxImporter->GetStatus().GetErrorString();
	}
	else
		FBXSDK_printf("\n\nSuccess reading FBX file!\n");


	//Create Scene container. This object will hold the loaded scene data. Fuck yes. 
	FbxScene* lScene = FbxScene::Create(lSdkManager, "tempScene");

	//Populate the shit out of that mother with the fresh data
	fbxImporter->Import(lScene);

	ReadSceneData(lScene);

	//After populating the scene file the importer can be EXTERMINATED
	fbxImporter->Destroy();
	lSdkManager->Destroy(); //Not sure bout this 1 tho


	//Time to have fun
	

	return 0;
}

bool Importer::ReadSceneData(FbxScene* scene)
{
	//Interpret all the data in the FBX Scene and fill our variables to prepare for export of custom file. 
	
	FbxNode* lRootNode = scene->GetRootNode();
	if (lRootNode)
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));

	return 0;
}

void Importer::PrintNode(FbxNode* pNode)
{
	//PrintTabs(); //IF YOU BOTHER TO MAKE THIS 
	const char* nodeName = pNode->GetName();

	cout << nodeName << "\n";
}