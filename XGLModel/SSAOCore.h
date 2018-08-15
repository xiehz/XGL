#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class IXMesh;
	class XGBuffer;
	class SSAOCore : public TutorialInterface
	{
	public:
		SSAOCore();
		~SSAOCore();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLint g_posmap;
		GLint g_samplerrad;
		GLint g_pers;
		GLint g_kernels;

		const static unsigned int KERNEL_SIZE = 64;
		void GenKernel();
	};


}

