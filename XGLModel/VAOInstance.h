#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class XVAOMesh;
	class VAOInstance : public TutorialInterface
	{
	public:
		VAOInstance();
		~VAOInstance();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLuint g_mvp;
		GLuint g_sampler2d;
		XVAOMesh* m_Mesh;
		XVAOMesh* m_Mesh1;
		float	 (*m_Instances)[16];
		const int N;

	};


}

