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

vector<Light>& Reader::GetLights()
{
    return lights;
}

vector<Camera>& Reader::GetCameras()
{
    return cameras;
}

vector<Transform>& Reader::GetTransforms()
{
    return transforms;
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
	//const char* fbxFilename = "Animation_Only_Preserve_bake.fbx";
	const char* fbxFilename = "sexyCube.fbx";

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

	//ANIMATION TESTINGANINATION TESTINGANINATION TESTINGANINATION TESTINGANINATION TESTING

	//https://gamedev.stackexchange.com/questions/59419/how-can-i-import-fbx-animations-using-the-fbx-sdk
	//https://forums.autodesk.com/t5/fbx-forum/how-do-i-get-animation-frames-and-keys-from-a-mesh/td-p/4189688
	//http://help.autodesk.com/view/FBX/2019/ENU/?guid=FBX_Developer_Help_animation_html

	FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
	int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();
	
	cout << "NumStacks: " << numStacks << " NUMSTACKS\n";

	for (int a = 0; a < numStacks; a++)
	{
		FbxAnimStack* animationStack = scene->GetSrcObject<FbxAnimStack>(a);
		int numAnimLayers = animationStack->GetMemberCount<FbxAnimLayer>();
		//cout << "NumStackSLAYERS: " << numAnimLayers << " NUMSTACKLAYERSZS\n";

		for (int n = 0; n < numAnimLayers; n++)
		{
			FbxAnimLayer* animationLayer = animationStack->GetMember<FbxAnimLayer>(n);
		}
	}

	//ANINATION TESTINGANINATION TESTINGANINATION TESTINGANINATION TESTINGANINATION TESTING

	//https://forums.autodesk.com/t5/fbx-forum/useful-things-you-might-want-to-know-about-fbxsdk/td-p/4821177
    if (lRootNode)
    {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)	//Traverse through every node in the scene.
        {
            //Fill our objects with data beep bop.  
            currentNode = lRootNode->GetChild(i);	//Get the node at hand

            //PrintNodeGeneralData(currentNode);	//Print each nodes data and it's kids, if it ever had any...

            if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
            {
                //meshes[i] = ProcessMesh(currentNode);
                Mesh mesh;
                Transform transform;
                ProcessMesh(currentNode, scene, mesh, transform);
                meshes.push_back(mesh);
                transforms.push_back(transform);

                globalHeader.transformCount += 1;
                globalHeader.meshCount += 1;
            }
            else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
            {
                Camera camera;
                Transform transform;
                ProcessCamera(currentNode, camera, transform);
                cameras.push_back(camera);
                transforms.push_back(transform);

                globalHeader.transformCount += 1;
                globalHeader.cameraCount += 1;
            }
            else if (currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eLight)
            {
                Light light;
                Transform transform;
                ProcessLight(currentNode, light, transform);
                lights.push_back(light);
                transforms.push_back(transform);

                globalHeader.transformCount += 1;
                globalHeader.lightCount += 1;
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
    }

	return 0;
}

void Reader::ProcessMesh(FbxNode* currentNode, FbxScene* scene, Mesh& omesh, Transform& otransform)
{
	Mesh tempMesh;
	Vertex tempVertex;
	FbxMesh* mesh = currentNode->GetMesh();	//Used GetGeometry before. Difference?
	int controlPointCount = mesh->GetControlPointsCount();
	int triangleCount = mesh->GetPolygonCount();
	int uvCount = mesh->GetTextureUVCount();
	int faceIndexCount = mesh->GetPolygonVertexCount();	//Vertex count * 3
	
	
	FbxVector4* controlPoints = mesh->GetControlPoints();
	tempMesh.controlPoints.reserve(controlPointCount);
	for (unsigned int i = 0; i < controlPointCount; i++)
	{
		array<float, 4> pos { controlPoints[i][0] , controlPoints[i][1] , controlPoints[i][2] , controlPoints[i][3] };
		tempMesh.controlPoints.push_back(pos);

	}

	int vertexNr = 0; //Keep track of the number of verts we fill our tempMesh with. 

	FbxLayerElementUV* meshUV = mesh->GetElementUV(0);	//Enable us to get the UV's from the UV Layer of the Mesh

	//Mesh tempMesh; //Fill this with the dada
	tempMesh.meshHeader.controlPointCount = controlPointCount;	//Fill the Mesh's-Header
	tempMesh.meshHeader.uvCount = uvCount;			//Fill the Mesh's-Header
	tempMesh.meshHeader.faceIndexCount = faceIndexCount;	//Fill the Mesh's-Header
	tempMesh.meshHeader.triangleCount = triangleCount;

	tempMesh.meshHeader.transformName = GetNodeTransform(currentNode).name;	//Get Mesh Transform

	//tempMesh.meshVertexList = new Vertex[faceIndexCount];
	
	//cout << "Triangle Count: " << triangleCount << "\n";
	//cout << "Vertex Count: " << vertexCount << "\n";
	//cout << "polyVert Count: " << faceIndexCount << "\n";
	//cout << "UV Count: " << uvCount << "\n\n";

	//RESERVED FOR ANIMATION TESTING RESERVED FOR ANIMATION TESTINGRESERVED FOR ANIMATION TESTING RESERVED FOR ANIMATION TESTING
	FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
	int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();

	cout << "NumStacks: " << numStacks << " NUMSTACKS\n";


	for (int a = 0; a < numStacks; a++)
	{
		FbxAnimStack* animationStack = scene->GetSrcObject<FbxAnimStack>(a);
		int numAnimLayers = animationStack->GetMemberCount<FbxAnimLayer>();
		//cout << "NumStackSLAYERS: " << numAnimLayers << " NUMSTACKLAYERSZS\n";

		for (int n = 0; n < numAnimLayers; n++)
		{
			FbxAnimLayer* animationLayer = animationStack->GetMember<FbxAnimLayer>(n);
			
			int animObjCount = animationLayer->GetDstObjectCount();

			for (int suck = 0; suck < animObjCount; suck++)
			{

				FbxObject* animObjectHopefully = animationLayer->GetDstObject(suck);
				cout << "AnimationLayer source object no: " << suck << "\n";
				cout << "Type: " << animObjectHopefully->GetTypeName() << "\n";
				cout << "Name: " << animObjectHopefully->GetName() << "\n";
				cout << "DstPropertyCount: " << animObjectHopefully->GetDstObjectCount() << "\n\n";

				for (int wah = 0; wah < animObjectHopefully->GetDstObjectCount(); wah++)
				{
					cout << "dstType: " << animObjectHopefully->GetDstObject(wah)->GetTypeName() << "\n";
					cout << "dstName: " << animObjectHopefully->GetDstObject(wah)->GetName() << "\n";
				}

			}

			cout << "ANIMOBJECTCOUNTPLEASE: " << animObjCount << " !\n";

			//FbxAnimCurve* animationCurve = 
		}

	}

	FbxGeometry* geometry = currentNode->GetGeometry();
	
	//FbxShape* shape = geometry->GetShapeCount
	//TO DO: BlendShapeChannels

	

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
	//RESERVED FOR ANIMATION TESTING RESERVED FOR ANIMATION TESTINGRESERVED FOR ANIMATION TESTING RESERVED FOR ANIMATION TESTING

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

    tempMesh.meshHeader.vertexCount = vertexNr;

	cout << "\nJust looped trough -" << vertexNr << "- polygon vertices in mesh -" << currentNode->GetName() << "-\n\n";
    omesh = tempMesh;
    otransform = GetNodeTransform(currentNode);
}

void Reader::ProcessCamera(FbxNode* currentNode, Camera& ocamera, Transform& otransform)
{
    FbxCamera* fbxCamera = currentNode->GetCamera();
    Camera tempCamera;

    tempCamera.transformName = GetNodeTransform(currentNode).name;		//Get Camera Transform

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

    ocamera = tempCamera;
    otransform = GetNodeTransform(currentNode);
}

void Reader::ProcessLight(FbxNode* currentNode, Light& olight, Transform& otransform)
{
    Light tempLight;
    FbxLight* fbxLight = currentNode->GetLight();

    tempLight.transformName = GetNodeTransform(currentNode).name;
    tempLight.color = fbxLight->Color.Get();
    tempLight.intensity = fbxLight->Intensity.Get();

    olight = tempLight;
    otransform = GetNodeTransform(currentNode);
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

Transform Reader::GetNodeTransform(FbxNode* currentNode)
{
	Transform tempTransform;
	tempTransform.name = currentNode->GetName();
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

		//cout << "Pos: " << cameras[t].cameraTransform.position.mData[0] << " "
		//	<< cameras[t].cameraTransform.position.mData[1] << " "
		//	<< cameras[t].cameraTransform.position.mData[2] << "\n";

		//cout << "Rot: " << cameras[t].cameraTransform.rotation.mData[0] << " "
		//	<< cameras[t].cameraTransform.rotation.mData[1] << " "
		//	<< cameras[t].cameraTransform.rotation.mData[2] << "\n";

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

		//cout << "Position: " << lights[l].lightTransform.position.mData[0] << " "
		//	<< lights[l].lightTransform.position.mData[1] << " "
		//	<< lights[l].lightTransform.position.mData[2] << "\n";

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