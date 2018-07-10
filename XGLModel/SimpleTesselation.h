#pragma once
#include "TutorialFactory.h"

namespace XGLModel {

	class SimpleTesselation : public TutorialInterface
	{
	public:
		SimpleTesselation();
		~SimpleTesselation();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
	private:
		GLint g_world;
		GLint g_view;
		GLint g_pers;
		GLint g_eye;

	};


}

