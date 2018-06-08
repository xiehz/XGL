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

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	private:
		IXMesh * m_pMesh;

		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;

	};

}

