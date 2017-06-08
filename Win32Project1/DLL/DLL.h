// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include "fbxsdk.h"
#include <vector>
#include <queue>


//
//// This class is exported from the DLL.dll
//class DLL_API CDLL
//{
//public:
//	CDLL(void);
//	// TODO: add your methods here.
//	void printTabs();
//	void printNode(FbxNode* pNode);
//	FbxString getAttributeTypeName(FbxNodeAttribute::EType type);
//	void printAttribute(FbxNodeAttribute* pAttribute);
//};
//
//extern DLL_API int nDLL;
//
//extern DLL_API int numTabs;
//
//DLL_API int fnDLL(void);

struct pos
{
	float xyzw[4];
};

struct FBXJoint
{
	FbxNode* node;
	int parentInd;
};

struct Joint
{
	float transform[16];
	int parentInd;
};



class fbx
{
private:
	FbxScene * fScene;
	FbxPose * fPose;
	const char* fileName = "Teddy_Idle.fbx";
	std::vector<pos> fMesh;
	std::vector<FBXJoint> fJoints;
	std::vector<Joint> returnJoints;

public:

	FbxString getAttributeTypeName(FbxNodeAttribute::EType type)
		{
			switch (type) {
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

	
	DLL_API bool loadScene()
	{

		

		FbxManager* fManager = FbxManager::Create();

		//Create the IO Settings

		FbxIOSettings *ios = FbxIOSettings::Create(fManager, IOSROOT);
		fManager->SetIOSettings(ios);

		//Create Importer

		FbxImporter* importer = FbxImporter::Create(fManager, "");

		if (!importer->Initialize(fileName, -1, fManager->GetIOSettings()))
		{
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
			return false;
		}

		fScene = FbxScene::Create(fManager, "myScene");

		importer->Import(fScene);

		importer->Destroy();

		return true;

	}

	void loadMeshData(FbxNode* node)
	{
		FbxMesh * mesh = node->GetMesh();

		std::vector<int> vertIndx;

		for (unsigned int i = 0; i < mesh->GetPolygonCount(); i++)
		{
			vertIndx.push_back(mesh->GetPolygonVertex(i, 0));
			vertIndx.push_back(mesh->GetPolygonVertex(i, 1));
			vertIndx.push_back(mesh->GetPolygonVertex(i, 2));

		}
		
		for (unsigned int i = 0; i < vertIndx.size(); i++)
		{
			FbxVector4 verts = mesh->GetControlPointAt(vertIndx[i]);

			pos tmp;

			tmp.xyzw[0] = verts.mData[0];
			tmp.xyzw[1] = verts.mData[1];
			tmp.xyzw[2] = verts.mData[2];
			tmp.xyzw[3] = verts.mData[3];

			fMesh.push_back(tmp);

		}


	}

	DLL_API void loadMesh()
	{
		FbxNode* root = fScene->GetRootNode();

		if (root)
		{
			for (unsigned int i = 0; i < root->GetChildCount(); i++)
			{
				FbxNode * child = root->GetChild(i);

				for (unsigned int j = 0; j < child->GetNodeAttributeCount(); j++)
				{

					if (getAttributeTypeName(child->GetNodeAttribute()->GetAttributeType()) == "mesh")
						loadMeshData(child);
				}

			}
		}



	}

	DLL_API std::vector<pos> getMesh()
	{
		return fMesh;
	}

	DLL_API void loadJoints()
	{
		

		for (unsigned int i = 0; i <  fScene->GetPoseCount(); i++)
		{
			if (fScene->GetPose(i)->IsBindPose())
			{
				fPose = fScene->GetPose(i);
			}
		}

		FbxNode* root;

		for (unsigned int i = 0; i < fPose->GetCount(); i++)
		{
			FbxSkeleton* tmp = fPose->GetNode(i)->GetSkeleton();

			if (tmp != nullptr)
			{
				if (tmp->IsSkeletonRoot())
				{
					root = fPose->GetNode(i);
					break;
				}
			}
		}

		std::queue<FBXJoint> nodes;
		FBXJoint tmp;
		tmp.node = root;
		tmp.parentInd = -1;
		nodes.push(tmp);

		while (!nodes.empty())
		{
			FBXJoint nTmp = nodes.front();
			nodes.pop();
			fJoints.push_back(nTmp);

			for (unsigned int i = 0; i < nTmp.node->GetChildCount(); i++)
			{
				FBXJoint jTmp;
				jTmp.node = nTmp.node->GetChild(i);
				jTmp.parentInd = fJoints.size() - 1;
				nodes.push(jTmp);
			}
		}

			for (unsigned int i = 0; i < fJoints.size(); i++)
			{
				Joint tmp;

				FbxMatrix trans = fJoints[i].node->EvaluateGlobalTransform();
				tmp.parentInd = fJoints[i].parentInd;
				for (unsigned int j = 0; j < 4; j++)
				{
					for (unsigned int k = 0; k < 4; k++)
					{
						tmp.transform[4 * k + j] = trans.Get(k, j);
					}
				}

				returnJoints.push_back(tmp);
			}
		}

	DLL_API std::vector<Joint> getJoints()
	{
		return returnJoints;
	}


};

	


