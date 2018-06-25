#pragma once

#include "TutorialFactory.h"

namespace XGLModel {
	class SkyCube;
	class XTexture;
	class Billboarding : public TutorialInterface
	{
	public:
		Billboarding();
		~Billboarding();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	private:
		GLuint g_eye;
		GLuint g_pers;
		GLuint g_sampler2d;
		SkyCube* m_pSky;
		XTexture* m_pTex;
	};


}

