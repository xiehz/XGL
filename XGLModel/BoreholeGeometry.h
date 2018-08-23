#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class XVAOMesh;
	class XTexture;
	class Background;
	class BoreholeGeometry : public TutorialInterface
	{
	public:
		BoreholeGeometry();
		~BoreholeGeometry();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLuint g_vp;
		GLuint g_sampler;
		float	 (*m_Instances)[3];
		const int N;

		GLuint m_VAO;
		GLuint m_DepthBuffer;
		XTexture* m_pTex;
		Background* m_pSky;

	};


}

