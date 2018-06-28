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

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		void setBillboardSize(float BillboardSize);
		virtual void draw() override;
		virtual void initUniform() override;
	public:
		GLuint g_eye;
		GLuint g_pers;
		GLuint g_sampler2d;
		GLuint g_billboardsize;

		SkyCube* m_pSky;
		XTexture* m_pTex;
	};


}

