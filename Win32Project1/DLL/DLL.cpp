//// DLL.cpp : Defines the exported functions for the DLL application.
////
//
#include "stdafx.h"
//
//
//// This is an example of an exported variable
//int nDLL = 0;
//int numTabs = 0;
//// This is an example of an exported function.
//int fnDLL(void)
//{
//	return 42;
//}
//
//// This is the constructor of a class that has been exported.
//// see DLL.h for the class definition
//CDLL::CDLL()
//{
//	return;
//}
//
//void DLL_API printTabs()
//{
//	for (int i = 0; i < numTabs; i++)
//		printf("\t");
//}
//
//FbxString CDLL::getAttributeTypeName(FbxNodeAttribute::EType type)
//{
//	switch (type) {
//	case FbxNodeAttribute::eUnknown: return "unidentified";
//	case FbxNodeAttribute::eNull: return "null";
//	case FbxNodeAttribute::eMarker: return "marker";
//	case FbxNodeAttribute::eSkeleton: return "skeleton";
//	case FbxNodeAttribute::eMesh: return "mesh";
//	case FbxNodeAttribute::eNurbs: return "nurbs";
//	case FbxNodeAttribute::ePatch: return "patch";
//	case FbxNodeAttribute::eCamera: return "camera";
//	case FbxNodeAttribute::eCameraStereo: return "stereo";
//	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
//	case FbxNodeAttribute::eLight: return "light";
//	case FbxNodeAttribute::eOpticalReference: return "optical reference";
//	case FbxNodeAttribute::eOpticalMarker: return "marker";
//	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
//	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
//	case FbxNodeAttribute::eBoundary: return "boundary";
//	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
//	case FbxNodeAttribute::eShape: return "shape";
//	case FbxNodeAttribute::eLODGroup: return "lodgroup";
//	case FbxNodeAttribute::eSubDiv: return "subdiv";
//	default: return "unknown";
//	}
//}
//
//void CDLL::printAttribute(FbxNodeAttribute * pAttribute)
//{
//	if (!pAttribute) return;
//
//	FbxString typeName = getAttributeTypeName(pAttribute->GetAttributeType());
//	FbxString attrName = pAttribute->GetName();
//	printTabs();
//	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
//	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
//}
//
//void CDLL::printNode(FbxNode * pNode)
//{
//	printTabs();
//	const char* nodeName = pNode->GetName();
//	FbxDouble3 translation = pNode->LclTranslation.Get();
//	FbxDouble3 rotation = pNode->LclRotation.Get();
//	FbxDouble3 scaling = pNode->LclScaling.Get();
//
//	// Print the contents of the node.
//	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
//		nodeName,
//		translation[0], translation[1], translation[2],
//		rotation[0], rotation[1], rotation[2],
//		scaling[0], scaling[1], scaling[2]
//	);
//	numTabs++;
//
//	// Print the node's attributes.
//	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
//		printAttribute(pNode->GetNodeAttributeByIndex(i));
//
//	// Recursively print the children.
//	for (int j = 0; j < pNode->GetChildCount(); j++)
//		printNode(pNode->GetChild(j));
//
//	numTabs--;
//	printTabs();
//	printf("</node>\n");
//}
//
//
