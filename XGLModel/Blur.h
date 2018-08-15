#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class XGBuffer;
	class Blur : public TutorialInterface
	{
	public:
		Blur();
		~Blur();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLint g_aomap;
	};


}

