#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class IXMesh;
	class AssimpImport : public TutorialInterface
	{
	public:
		AssimpImport();
		virtual ~AssimpImport();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		IXMesh * m_pMesh;

		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;

	};

}

