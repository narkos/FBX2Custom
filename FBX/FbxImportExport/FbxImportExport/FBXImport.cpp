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
	//const char* fbxFilename = "testFileFBX.fbx";
	const char* fbxFilename = "testFileFBX_triangulated.fbx";

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

	ReadSceneData(lScene); //Populate all of our data storage so that it can be written as our custom files!

	//After populating the scene file the importer can be EXTERMINATED
	fbxImporter->Destroy();
	lSdkManager->Destroy(); //Not sure bout this 1 tho
	
	return 0;
}

bool Importer::ReadSceneData(FbxScene* scene)
{
	//Interpret all the data in the FBX Scene and fill our variables to prepare for export of custom file. 

	FbxNode* lRootNode = scene->GetRootNode();
	cout << "Number of objects: " << lRootNode->GetChildCount() << "\n\n";

	FbxNode* currentNode; //Will get refilled for every object

	//Create object array[no of nodes]

	//WOW READ
	//https://forums.autodesk.com/t5/fbx-forum/useful-things-you-might-want-to-know-about-fbxsdk/td-p/4821177
	if (lRootNode)
		for (int i = 0; i < lRootNode->GetChildCount(); i++)	//Traverse through every node in the scene.
		{
			//Fill our objects with data beep bop.  
			currentNode = lRootNode->GetChild(i);	//Get the node at hand
			//objectArray[i].position = currentNode.pos typ

			//Fill General Node Information, regardless of node type
			nodeName = currentNode->GetName();
			nodeTransformData.position = currentNode->LclTranslation.Get();
			nodeTransformData.rotation = currentNode->LclRotation.Get();
			nodeTransformData.scale = currentNode->LclScaling.Get();

			PrintNodeGeneralData(currentNode);	//Print each nodes data and it's kids, if it ever had any...

			if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				globalHeader.meshCount += 1;	//Increment count
				ProcessMesh(currentNode);
			}

			if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
			{
				globalHeader.cameraCount += 1;	//Increment count

			}

			if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eLight)
			{
				globalHeader.lightCount += 1;	//Increment count

			}
			//How to get TEXTURES/MATERIALS?? It's no node
		}

	return 0;
}

void Importer::ProcessMesh(FbxNode* currentNode)
{
	FbxMesh* mesh = currentNode->GetMesh();	//Used GetGeometry before. Difference?
	int vertexCount = mesh->GetControlPointsCount();
	int triangleCount = mesh->GetPolygonCount();
	int normalCount = mesh->GetElementNormalCount();
	int uvCount = mesh->GetElementUVCount();
	int texUVCount = mesh->GetTextureUVCount();
	int faceIndexCount = mesh->GetPolygonVertexCount();	//Vertex count * 3

	FbxLayerElementUV* meshUV = mesh->GetElementUV(0);	//Enable us to get the UV's from the UV Layer of the Mesh
	//FbxLayerElementTangent* meshTangent = mesh->GetElementTangent(0);
	

	//Mesh meshData; //Fill this with the dada
	meshData.meshHeader.vertexCount = vertexCount;	//Fill the Mesh's-Header
	meshData.meshHeader.normalCount = normalCount;	//Fill the Mesh's-Header
	meshData.meshHeader.uvCount = uvCount;			//Fill the Mesh's-Header
	meshData.meshHeader.faceIndexCount = faceIndexCount;	//Fill the Mesh's-Header

	//meshData.meshVertexList = new Vertex[vertexCount];	//USE IF WE DONT GO THROUGH TRIANGLES
	meshData.meshVertexList = new Vertex[faceIndexCount];
	int vertexNr = 0; //Keep track of the number of verts we fill our MeshData with. 

	cout << "Triangle Count: " << triangleCount << "\n";
	cout << "Vertex Count: " << vertexCount << "\n";
	cout << "polyVert Count: " << faceIndexCount << "\n";
	cout << "Normal Count: " << normalCount << "\n";
	cout << "UV Count: " << uvCount << "\n";
	cout << "Texture UV Count: " << texUVCount << "\n\n";

	for (int j = 0; j < triangleCount; j++)	//For every triangle of the mesh
	{
		//https://stackoverflow.com/questions/50926809/fetching-indices-with-fbx-sdk
		for (int x = 0; x < 3; x++)	//Traverse through every vertex of every triangle 
		{
			int index = mesh->GetPolygonVertex(j, x);
			//cout << index << " ";
			//cout << vertexNr << " ";

			//GET VERTEX POS. MAYBE MAKE A FUNCTION LOL 
			meshData.meshVertexList[vertexNr].vPos = mesh->GetControlPointAt(index); //USE IF WE DONT GO THROUGH TRIANGLES
			//cout << "vPos: " << meshData.meshVertexList[vertexNr].vPos[0] << " " << meshData.meshVertexList[vertexNr].vPos[1] << " " << meshData.meshVertexList[vertexNr].vPos[2] << " \n";

			//HEY LOOK I MADE A FUNCTION LOL
			ProcessNormals(mesh, index, vertexNr);

			int UVIndex = mesh->GetTextureUVIndex(j, x);

			//meshData.meshVertexList[vertexNr].vTangent = meshTangent->GetDirectArray().GetAt(index);
			//cout << "vTan: " << meshData.meshVertexList[vertexNr].vTangent[0] << " " << meshData.meshVertexList[vertexNr].vTangent[1] << " " << meshData.meshVertexList[vertexNr].vTangent[2] << " \n";


			//GET VERTEX UV. MAYBE MAKE A FUNCTION LOL
			//meshData.meshVertexList[vertexNr].vUV = meshUV->GetDirectArray().GetAt(index);
			meshData.meshVertexList[vertexNr].vUV = meshUV->GetDirectArray().GetAt(UVIndex);
			//cout << "vUV: " << meshData.meshVertexList[vertexNr].vUV[0] << " " << meshData.meshVertexList[vertexNr].vUV[1] << " \n";


			vertexNr++;
		}
		//cout << "\n";
	}
	cout << "Just looped trough -" << vertexNr << "- polygon vertices in mesh -" << nodeName << "-\n\n";
}

void Importer::ProcessNormals(FbxMesh* mesh, int index, int vertexNr)
{
	FbxGeometryElementNormal* meshNormal = mesh->GetElementNormal(0);	//Enable us to get the Normals from the Normal Layer of the Mesh


	if (meshNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		meshData.meshVertexList[vertexNr].vNormal = meshNormal->GetDirectArray().GetAt(index);
		//cout << "Normal Mapping mode: ByControlPoint\n";
	}
	else if (meshNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		meshData.meshVertexList[vertexNr].vNormal = meshNormal->GetDirectArray().GetAt(vertexNr);
		//cout << "Normal Mapping mode: ByPolygonVertex\n";
	}
	//cout << "vNor: " << meshData.meshVertexList[vertexNr].vNormal[0] << " " << meshData.meshVertexList[vertexNr].vNormal[1] << " " << meshData.meshVertexList[vertexNr].vNormal[2] << " \n";
}

void Importer::PrintNodeGeneralData(FbxNode* pNode)
{
	//PrintTabs(); //IF YOU BOTHER TO MAKE THIS 
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scale = pNode->LclScaling.Get();

	cout << "Node Name: " << nodeName << "\n";
	cout << "Translation: " << translation[0] << ", " << translation[1] << ", " << translation[2] << "\n";
	cout << "Rotation : " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << "\n";
	cout << "Scale: " << scale[0] << ", " << scale[1] << ", " << scale[2] << "\n";

	//Print the current node attributes
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	//Recursively Print the current node kids
	for (int j = 0; j < pNode->GetChildCount(); j++)
		PrintNodeGeneralData(pNode->GetChild(j));
}

void Importer::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();

	cout << "Node type: " << typeName.Buffer() << "\n";
	cout << "Attr type: " << attrName.Buffer() << "\n\n";

}

FbxString Importer::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{
	switch (type) 
	{
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
	}
}