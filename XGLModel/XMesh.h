#pragma once

#include "IXMesh.h"

namespace XGLModel {

	class XMesh : public IXMesh
	{
	public:
		XMesh();
		virtual ~XMesh();
	public:
		virtual bool LoadMesh(const std::string& filename);
		virtual void Render();
	protected:
		virtual void InitMesh(unsigned int index, const aiMesh* pMesh);
	};


}

