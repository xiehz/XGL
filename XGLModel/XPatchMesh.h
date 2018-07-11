#pragma once
#include "IXMesh.h"
namespace XGLModel {
	class XPatchMesh : public IXMesh
	{
	public:
		XPatchMesh();
		~XPatchMesh();
		virtual void Render() override;
	};
}

