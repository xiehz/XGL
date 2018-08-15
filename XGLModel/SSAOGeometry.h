#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class XGBuffer;
	class SSAOGeometry : public TutorialInterface
	{
	public:
		SSAOGeometry();
		~SSAOGeometry();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLint g_mv;
		GLint g_mvp;
	};


}

