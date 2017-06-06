#pragma once
#include "fbxsdk.h"

class FBX
{
public:
	FBX();
	~FBX();

	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void PrintTabs();
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintNode(FbxNode* pNode);



private:

};

FBX::FBX()
{
}

FBX::~FBX()
{
}