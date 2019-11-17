#include "FBXImport.h"
#include <string.h>

using namespace std;

Reader::Reader()
{
	//Initialize Here
}

Reader::~Reader()
{
	//Destroy Here

}

FbxFileHeader* Reader::GetHeader()
{
	return &globalHeader;
}

vector<Mesh> &Reader::GetMeshes()
{
	return meshes;
}

bool Reader::ImportFBX()
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
	//const char* fbxFilename = "testFileFBX_triangulated.fbx";
	//const char* fbxFilename = "testFileFBX_triangulated_Lights.fbx";
	//const char* fbxFilename = "testFileFBX_blend_shapes.fbx";
	//const char* fbxFilename = "Animation_Only.fbx";
	//const char* fbxFilename = "Animation_Only_Baked.fbx";
	//const char* fbxFilename = "testFileFBX_Animation_Layer_bake.fbx";
	//const char* fbxFilename = "BlendshapeTest.fbx";
	const char* fbxFilename = "Animation_Only_Preserve_bake.fbx";
	//const char* fbxFilename = "sexyCube.fbx";
	//const char* fbxFilename = "VERYCOOLSHAPE.fbx";
	//const char* fbxFilename = "diamond.fbx";
	//const char* fbxFilename = "blendamore.fbx";

	//Initialize the damn importher. 
	bool lImportStatus = fbxImporter->Initialize(fbxFilename, -1, lSdkManager->GetIOSettings());

	if (!lImportStatus)
	{
		cout << "\nSHIT, Error whilst initializing the importer:\n";
		cout << fbxImporter->GetStatus().GetErrorString();
	}
	else
		FBXSDK_printf("Success reading FBX file!\n");

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

bool Reader::ReadSceneData(FbxScene* scene)
{
	//Interpret all the data in the FBX Scene and fill our variables to prepare for export of custom file. 

	FbxNode* lRootNode = scene->GetRootNode();
	cout << "Number of objects: " << lRootNode->GetChildCount() << "\n\n";

	FbxNode* currentNode; //Will get refilled for every object

	//https://forums.autodesk.com/t5/fbx-forum/useful-things-you-might-want-to-know-about-fbxsdk/td-p/4821177
	if (lRootNode)

		//New Animation testing.
		ReadAnimationLayers(scene);	//Might wanna return something later

		for (int i = 0; i < lRootNode->GetChildCount(); i++)	//Traverse through every node in the scene.
		{
			//Fill our objects with data beep bop.  
			currentNode = lRootNode->GetChild(i);	//Get the node at hand
			//currentNode = lRootNode;	//Get the node at hand
	
			//PrintNodeGeneralData(currentNode);	//Print each nodes data and it's kids, if it ever had any...

			//ReadDeformerData(currentNode); //Sends ANY node looking for deformer data.

			if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				globalHeader.meshCount += 1;	//Increment count
				//meshes[i] = ProcessMesh(currentNode);
				//ReadDeformerData(currentNode); //Sends MESH node looking for deformer data.
				meshes.push_back(ProcessMesh(currentNode, scene));
			}
			else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
			{
				globalHeader.cameraCount += 1;	//Increment count
				cameras.push_back(ProcessCamera(currentNode));
			}
			else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eLight)
			{
				globalHeader.lightCount += 1;	//Increment count
				lights.push_back(ProcessLight(currentNode));
			}
			else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eShape)
			{
				cout << "";
			}
			else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eUnknown)
			{
				cout << "";
			}
			else
			{
				cout << "Theres something else!";
				PrintAttribute(currentNode->GetNodeAttributeByIndex(0));
			}
			//How to get TEXTURES/MATERIALS?? It's no node
		}

	return 0;
}

Mesh Reader::ProcessMesh(FbxNode* currentNode, FbxScene* scene)
{
	Mesh tempMesh;
	Vertex tempVertex;
	FbxMesh* mesh = currentNode->GetMesh();	//Used GetGeometry before. Difference?
	int vertexCount = mesh->GetControlPointsCount();
	int triangleCount = mesh->GetPolygonCount();
	int uvCount = mesh->GetTextureUVCount();
	int faceIndexCount = mesh->GetPolygonVertexCount();	//Vertex count * 3
	
	
	FbxVector4* controlPoints = mesh->GetControlPoints();
	tempMesh.controlPoints.reserve(vertexCount);
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		array<float, 4> pos { controlPoints[i][0] , controlPoints[i][1] , controlPoints[i][2] , controlPoints[i][3] };
		tempMesh.controlPoints.push_back(pos);

	}


	int vertexNr = 0; //Keep track of the number of verts we fill our tempMesh with. 

	FbxLayerElementUV* meshUV = mesh->GetElementUV(0);	//Enable us to get the UV's from the UV Layer of the Mesh

	//Mesh tempMesh; //Fill this with the dada
	tempMesh.meshHeader.vertexCount = vertexCount;	//Fill the Mesh's-Header
	tempMesh.meshHeader.uvCount = uvCount;			//Fill the Mesh's-Header
	tempMesh.meshHeader.faceIndexCount = faceIndexCount;	//Fill the Mesh's-Header
	tempMesh.meshHeader.triangleCount = triangleCount;

	tempMesh.meshTransform = GetNodeTransform(currentNode);	//Get Mesh Transform

	//tempMesh.meshVertexList = new Vertex[faceIndexCount];
	
	//cout << "Triangle Count: " << triangleCount << "\n";
	//cout << "Vertex Count: " << vertexCount << "\n";
	//cout << "polyVert Count: " << faceIndexCount << "\n";
	//cout << "UV Count: " << uvCount << "\n\n";



	FbxGeometry* geometry = currentNode->GetGeometry();
	
	//FbxShape* shape = geometry->GetShapeCount
	//TO DO: BlendShapeChannels

	int geometryLayerCount = geometry->GetLayerCount();
	if (geometryLayerCount > 0)
	{
		FbxLayer* geoLayer = geometry->GetLayer(0);
		//geoLayer->
	}

	if (mesh->GetShapeCount() > 0)
	{
		int countt = geometry->GetShapeCount();
	}

	if (geometry->GetDeformerCount() > 0)
		cout << "\nWhaaaadup";

	if (geometry->GetShapeCount() > 0)
		cout << "\n HOOOL UP\n";

	if (mesh->GetDeformerCount() > 0)
	{
		FbxDeformer* deformer = mesh->GetDeformer(0);
		if (deformer->GetDeformerType() == FbxDeformer::eBlendShape)
			cout << "\n\n\n\n\nHurray\n\n\n\n\n\n";
	}

	//Alternative if we just do a short list of vertices:
	//for (int j = 0; j < vertexCount; j++)
	//{
	//	tempVertex.vPos = mesh->GetControlPointAt(j);
	//	tempVertex.vNormal = mesh->GetElementNormal(0)->GetDirectArray().GetAt(j);
	//	tempVertex.vUV = meshUV->GetDirectArray().GetAt(j);
	//	//tempVertex.vTangent = 
	//}

	for (int j = 0; j < triangleCount; j++)	//For every triangle of the mesh
	{
		//https://stackoverflow.com/questions/50926809/fetching-indices-with-fbx-sdk
		//https://www.gamedev.net/articles/programming/graphics/how-to-work-with-fbx-sdk-r3582
		for (int x = 0; x < 3; x++)	//Traverse through every vertex of every triangle 
		{
			int index = mesh->GetPolygonVertex(j, x);
			int UVIndex = mesh->GetTextureUVIndex(j, x);
			//cout << index << " ! " << UVIndex << "\n";
			//cout << vertexNr << " ";

			tempMesh.meshIndices.push_back(index);	//Fill the mesh index list

			tempVertex.vPos = mesh->GetControlPointAt(index); //Get Vertex Position
			tempVertex.vNormal = ProcessNormals(mesh, index, vertexNr);	//Get vertex Normal
			tempVertex.vUV = meshUV->GetDirectArray().GetAt(UVIndex);	//Get Vertex UV
			tempVertex.vTangent = ProcessTangents(mesh, index, vertexNr);
			//TO Do: Bi_Normals
			//tempVertex.vTangent = meshTangent->GetDirectArray().GetAt(index);

			vertexNr++;
			tempMesh.meshVertices.push_back(tempVertex);
		}
	}

	cout << "\nJust looped trough -" << vertexNr << "- polygon vertices in mesh -" << currentNode->GetName() << "-\n\n";
	return tempMesh;
}

FbxDouble3 Reader::ProcessNormals(FbxMesh* mesh, int index, int vertexNr)
{
	if (mesh->GetElementNormalCount() < 1)	//Failsafe
		return FbxDouble3(0, 0, 0);

	//ELSE
	FbxGeometryElementNormal* meshNormal = mesh->GetElementNormal(0);	//Enable us to get the Normals from the Normal Layer of the Mesh
	FbxDouble3 tempNormal;

	if (meshNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		//tempMesh.meshVertexList[vertexNr].vNormal = meshNormal->GetDirectArray().GetAt(index);	//OLD
		tempNormal = meshNormal->GetDirectArray().GetAt(index);
		//cout << "ControlP ";
		//cout << "Normal Mapping mode: ByControlPoint\n";
	}
	else if (meshNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		tempNormal = meshNormal->GetDirectArray().GetAt(vertexNr);
		//cout << "PolVert ";
		//cout << "Normal Mapping mode: ByPolygonVertex\n";
	}
	return tempNormal;
}

FbxDouble3 Reader::ProcessTangents(FbxMesh* mesh, int index, int vertexNr)
{
	if (mesh->GetElementTangentCount() < 1)	//Failsafe
		return FbxDouble3(0, 0, 0);

	//ELSE:
	FbxLayerElementTangent* meshTangent = mesh->GetElementTangent(0);	//Enable us to get the Tangents from the Tangents Layer of the Mesh
	FbxDouble3 tempTangent;

	if (meshTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		tempTangent = meshTangent->GetDirectArray().GetAt(index);
	else if (meshTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		tempTangent = meshTangent->GetDirectArray().GetAt(vertexNr);

	return tempTangent;
}

Camera Reader::ProcessCamera(FbxNode* currentNode)
{
	FbxCamera* fbxCamera = currentNode->GetCamera();
	Camera tempCamera;

	tempCamera.cameraTransform = GetNodeTransform(currentNode);		//Get Camera Transform

	if (fbxCamera->ProjectionType.Get() == FbxCamera::EProjectionType::eOrthogonal)
		tempCamera.isOrtho = true;
	else
		tempCamera.isOrtho = false;	//Perspective (ePerspective)

	tempCamera.viewDirection = fbxCamera->EvaluateLookAtPosition();
	tempCamera.upVector = fbxCamera->UpVector.Get();
	tempCamera.projectionMatrix = fbxCamera->ComputeProjectionMatrix(1920, 1080);	//Ehhh, these numbers should be???
	//PrintFbxMatrix(tempCamera.projectionMatrix);
	//TO DO: FOV?
	//fbxCamera->FieldOfViewX.Get()

	return tempCamera;
}

Light Reader::ProcessLight(FbxNode* currentNode)
{
	Light tempLight;
	FbxLight* fbxLight = currentNode->GetLight();

	tempLight.lightTransform = GetNodeTransform(currentNode);
	tempLight.color = fbxLight->Color.Get();
	tempLight.intensity= fbxLight->Intensity.Get();

	return tempLight;
}

Transform Reader::GetNodeTransform(FbxNode* currentNode)
{
	Transform tempTransform;
	tempTransform.name = currentNode->GetName();
	tempTransform.parentName = currentNode->GetParent()->GetName();
	tempTransform.position = currentNode->LclTranslation.Get();
	tempTransform.rotation = currentNode->LclRotation.Get();
	tempTransform.scale = currentNode->LclScaling.Get();
	return tempTransform;
}

void Reader::PrintNodeGeneralData(FbxNode* pNode)
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

void Reader::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();

	cout << "Node type: " << typeName.Buffer() << "\n";
	cout << "Attr type: " << attrName.Buffer() << "\n\n";

}

void Reader::PrintTestData()
{
	cout << "---->>>>>>>>>> TEST DATA BELOW <<<<<<<<<<----\n";
	cout << ">>>>>>>>>>MESHES<<<<<<<<<<\n";
	int nrMeshes = meshes.size();

	//cout << "Lenght of Meshes Vector Array: " << nrMeshes << "\n";

	for (int i = 0; i < nrMeshes; i++)
	{
		int nrVertices = meshes[i].meshVertices.size();
		cout << "\nMesh no." << i << " got " << nrVertices << " vertices\n";

		//for (int j = 0; j < nrVertices; j++)
		//{
		//	//VERTEX POSITION
		//	cout << "Pos: " << meshes[i].meshVertices[j].vPos.mData[0] << " "
		//		<< meshes[i].meshVertices[j].vPos.mData[1] << " "
		//		<< meshes[i].meshVertices[j].vPos.mData[2] << "\n";

		//	//VERTEX NORMAL
		//	cout << "Normal: " << meshes[i].meshVertices[j].vNormal.mData[0] << " "
		//		<< meshes[i].meshVertices[j].vNormal.mData[1] << " "
		//		<< meshes[i].meshVertices[j].vNormal.mData[2] << "\n";

		//	//VERTEX UV
		//	cout << "UV: " << meshes[i].meshVertices[j].vUV.mData[0] << " "
		//		<< meshes[i].meshVertices[j].vUV.mData[1] << "\n";

		//	//VERTEX TANGENT
		//	cout << "Tan: " << meshes[i].meshVertices[j].vTangent.mData[0] << " "
		//		<< meshes[i].meshVertices[j].vTangent.mData[1] << " "
		//		<< meshes[i].meshVertices[j].vTangent.mData[2] << "\n";
		//}
	}

	cout << "\n>>>>>>>>>>CAMERAS<<<<<<<<<<\n";
	int nrCameras = cameras.size();
	for (int t = 0; t < nrCameras; t++)
	{
		cout << "Camera nr " << t << "\n";

		cout << "Pos: " << cameras[t].cameraTransform.position.mData[0] << " "
			<< cameras[t].cameraTransform.position.mData[1] << " "
			<< cameras[t].cameraTransform.position.mData[2] << "\n";

		cout << "Rot: " << cameras[t].cameraTransform.rotation.mData[0] << " "
			<< cameras[t].cameraTransform.rotation.mData[1] << " "
			<< cameras[t].cameraTransform.rotation.mData[2] << "\n";

		cout << "View Direction: " << cameras[t].viewDirection.mData[0] << " "
			<< cameras[t].viewDirection.mData[1] << " "
			<< cameras[t].viewDirection.mData[2] << "\n";

		cout << "Up Vector: " << cameras[t].upVector.mData[0] << " "
			<< cameras[t].upVector.mData[1] << " "
			<< cameras[t].upVector.mData[2] << "\n";

		if (cameras[t].isOrtho == false)
			cout << "Projection type: Perspective\n";
		else 
			cout << "Projection type: Orthographic\n";

		PrintFbxMatrix(cameras[t].projectionMatrix);
	}

	cout << "\n>>>>>>>>>>LIGHTS<<<<<<<<<<\n";
	int nrLights = lights.size();
	for (int l = 0; l < nrLights; l++)
	{
		cout << "Light nr " << l << "\n";

		cout << "Position: " << lights[l].lightTransform.position.mData[0] << " "
			<< lights[l].lightTransform.position.mData[1] << " "
			<< lights[l].lightTransform.position.mData[2] << "\n";

		cout << "Color: " << lights[l].color.mData[0] << " "
			<< lights[l].color.mData[1] << " "
			<< lights[l].color.mData[2] << "\n";

		cout << "Intensity: " << lights[l].intensity << "\n";
	}
}

void Reader::PrintFbxMatrix(FbxMatrix inMatrix)
{
	cout << "\nMATRIX:\n";
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			cout << inMatrix.mData[a].mData[b] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

FbxString Reader::GetAttributeTypeName(FbxNodeAttribute::EType type)
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

void Reader::ReadAnimationLayers(FbxScene* scene)
{
	////https://gamedev.stackexchange.com/questions/59419/how-can-i-import-fbx-animations-using-the-fbx-sdk
	////https://forums.autodesk.com/t5/fbx-forum/how-do-i-get-animation-frames-and-keys-from-a-mesh/td-p/4189688
	////http://help.autodesk.com/view/FBX/2019/ENU/?guid=FBX_Developer_Help_animation_html

	FbxNode* currentNode = scene->GetRootNode();
	int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();
	//FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
	cout << "Number of animation Stacks: " << numStacks << "\n";
	
	for (int a = 0; a < numStacks; a++) //For every animation stack
	{
		FbxAnimStack* animationStack = scene->GetSrcObject<FbxAnimStack>(a);
		int numAnimLayers = animationStack->GetMemberCount<FbxAnimLayer>();
		cout << "Current animation stack: " << a+1 << "\n";
		cout << "Animation Stack Name: " << animationStack->GetName() << "\n";
		cout << "Number of animation layers in current stack: " << numAnimLayers << "\n";

		for (int n = 0; n < numAnimLayers; n++) //For every animation layer in current stack
		{
			FbxAnimLayer* animationLayer = animationStack->GetMember<FbxAnimLayer>(n);
			int modelCount;


			//ReadAnimationCurves(animationLayer, currentNode);
			for (modelCount = 0; modelCount < currentNode->GetChildCount(); modelCount++)
			{
				ReadAnimationCurves(animationLayer, currentNode->GetChild(modelCount));
			}
		}
	}
}

void Reader::ReadAnimationCurves(FbxAnimLayer* currentLayer, FbxNode* currentNode)
{
	cout << "\nNode Name: " << currentNode->GetName() << "\n";
	FbxAnimCurve* lAnimCurve = NULL;
	FbxProperty lProperty = currentNode->GetFirstProperty();

	FbxAnimCurveNode* lAnimCurveNode = lProperty.GetCurveNode(currentLayer);

	//if (lAnimCurveNode->IsAnimated())
	//{
	//	cout << "CurveNode: " << lAnimCurveNode->GetName() << "\n";
	//}


	lAnimCurve = currentNode->LclTranslation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if (lAnimCurve)
		ExtractCurveData(lAnimCurve);
	lAnimCurve = currentNode->LclTranslation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (lAnimCurve)
		ExtractCurveData(lAnimCurve);
	lAnimCurve = currentNode->LclTranslation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (lAnimCurve)
		ExtractCurveData(lAnimCurve);

	//lAnimCurve = currentNode->LclRotation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_X);
	//if (lAnimCurve)
	//	ExtractCurveData(lAnimCurve);
	//lAnimCurve = currentNode->LclRotation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	//if (lAnimCurve)
	//	ExtractCurveData(lAnimCurve);
	//lAnimCurve = currentNode->LclRotation.GetCurve(currentLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	//if (lAnimCurve)
	//	ExtractCurveData(lAnimCurve);

	//lAnimCurve = currentNode->GetCurve()
	//FBXSDK_CURVENODE_TRANSFORM

	if (lAnimCurve)
		ExtractCurveData(lAnimCurve);

	ReadDeformerData(currentNode);

	//PrintListCurveKeys(lAnimCurve, NULL);

	int animObjCount = currentLayer->GetDstObjectCount();
	cout << "Animation layer name: " << currentLayer->GetName() << "\n";
	cout << "Number of Destination objects for this layer: " << animObjCount << "\n\n";

	for (int suck = 0; suck < animObjCount; suck++)
	{
		FbxObject* animObjectHopefully = currentLayer->GetDstObject(suck);
		cout << "Destination object no: " << suck << "\n";
		cout << "Type: " << animObjectHopefully->GetTypeName() << "\n";
		cout << "Name: " << animObjectHopefully->GetName() << "\n";
		//cout << "DstPropertyCount: " << animObjectHopefully->GetDstObjectCount() << "\n\n";

		for (int wah = 0; wah < animObjectHopefully->GetDstObjectCount(); wah++)
		{
			//cout << "dstType: " << animObjectHopefully->GetDstObject(wah)->GetTypeName() << "\n";
			//cout << "dstName: " << animObjectHopefully->GetDstObject(wah)->GetName() << "\n";
			FbxProperty lProperty = animObjectHopefully->GetFirstProperty();
			FbxAnimEvaluator* animEvaluator = lProperty.GetAnimationEvaluator();
			//cout << "Animation Evaluator Data: " << animEvaluator->GetDstObjectCount() << "\n"
			//	<< animEvaluator->GetDstPropertyCount() << "\n"
			//	<< animEvaluator->GetNameOnly() << "\n";
		}

	}
	//cout << "ANIMOBJECTCOUNTPLEASE: " << animObjCount << " !\n"; 
}

void Reader::ExtractCurveData(FbxAnimCurve* curve)
{
	cout << "Haha I'm extraxtracting curve data \n";
	
	FbxTime keyTime;
	float keyValue;
	char timeString[256];
	int counter;
	
	int keyCount = curve->KeyGetCount();
	cout << "Keycount: " << keyCount << "\n";

	for (counter = 0; counter < keyCount; counter++)	//For every Key on this curve
	{
		keyValue = static_cast<float>(curve->GetValue(counter));
		keyTime = curve->KeyGetTime(counter);

		cout << "    Key Time: " << keyTime.GetTimeString(timeString, FbxUShort(256));
		cout << " ...Key Value: " << keyValue << "\n";
	}
}

void Reader::ReadDeformerData(FbxNode* currentNode)
{
	FbxNodeAttribute* lNodeAttribute = currentNode->GetNodeAttribute();

	if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
	{
		//FbxGeometry* lGeometry = (FbxGeometry*)lNodeAttribute;
		FbxGeometry* lGeometry = currentNode->GetGeometry();
		FbxMesh* lMesh = currentNode->GetMesh();

		int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape); //eVertexCache eUnknown eSkin eBlendShape
		//cout << "Blend Shape deformer count: " << lBlendShapeDeformerCount << "\n";
		int lVertexDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eVertexCache); //eVertexCache eUnknown eSkin eBlendShape
		//cout << "Vertex Cache deformer count: " << lVertexDeformerCount << "\n";
		int lSkinDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eSkin); //eVertexCache eUnknown eSkin eBlendShape
		//cout << "Skin deformer count: " << lSkinDeformerCount << "\n";
		int lUnknownDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eUnknown); //eVertexCache eUnknown eSkin eBlendShape
		//cout << "Unknown deformer count: " << lUnknownDeformerCount << "\n";

		cout << "Deformer counts: " << lBlendShapeDeformerCount << ", " << lVertexDeformerCount << ", " << lSkinDeformerCount << ", " << lUnknownDeformerCount << "\n";

		if (lBlendShapeDeformerCount > 0 | lVertexDeformerCount > 0 | lSkinDeformerCount > 0 | lUnknownDeformerCount > 0)
		{
			cout << "BreakIT";
		}
	}
}

//void Reader::PrintListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
//{
//	FbxTime   lKeyTime;
//	int     lKeyValue;
//	char    lTimeString[256];
//	FbxString lListValue;
//	FbxString lOutputString;
//	int     lCount;
//
//	int lKeyCount = pCurve->KeyGetCount();
//
//	for (lCount = 0; lCount < lKeyCount; lCount++)
//	{
//		lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
//		lKeyTime = pCurve->KeyGetTime(lCount);
//
//		lOutputString = "            Key Time: ";
//		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
//		lOutputString += ".... Key Value: ";
//		lOutputString += lKeyValue;
//		lOutputString += " (";
//		lOutputString += pProperty->GetEnumValue(lKeyValue);
//		lOutputString += ")";
//
//		lOutputString += "\n";
//		FBXSDK_printf(lOutputString);
//	}
//}
//
//void Reader::DisplayCurveKeys(FbxAnimCurve* pCurve)
//{
//	static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
//	static const char* constantMode[] = { "?", "Standard", "Next" };
//	static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
//	static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };
//
//	FbxTime   lKeyTime;
//	float   lKeyValue;
//	char    lTimeString[256];
//	FbxString lOutputString;
//	int     lCount;
//
//	int lKeyCount = pCurve->KeyGetCount();
//
//	for (lCount = 0; lCount < lKeyCount; lCount++)
//	{
//		lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
//		lKeyTime = pCurve->KeyGetTime(lCount);
//
//		lOutputString = "            Key Time: ";
//		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
//		lOutputString += ".... Key Value: ";
//		lOutputString += lKeyValue;
//		lOutputString += " [ ";
//		lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
//		if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
//		{
//			lOutputString += " | ";
//			lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
//		}
//		else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
//		{
//			lOutputString += " | ";
//			lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
//			lOutputString += " | ";
//			lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
//			lOutputString += " | ";
//			lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
//		}
//		lOutputString += " ]";
//		lOutputString += "\n";
//		FBXSDK_printf(lOutputString);
//	}
//}