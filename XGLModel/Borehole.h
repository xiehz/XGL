#pragma once

#include "TutorialFactory.h"

namespace XGLModel {

	class BoreoheMesh;
	class Background;
	class Borehole : public TutorialInterface
	{
	public:
		Borehole();
		~Borehole();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		GLuint g_mvp;
		GLuint g_sampler2d;
		BoreoheMesh* m_Mesh;
		float	 (*m_Instances)[16];
		const int N;
		Background* m_pSky;
	};


}

