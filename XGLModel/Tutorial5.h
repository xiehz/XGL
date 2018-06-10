#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	class Tutorial5 :
		public TutorialInterface
	{
	public:
		Tutorial5();
		virtual ~Tutorial5();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;
	private:
		GLint uniform_scale;
		GLint uniform_model_matrix;
		GLint uniform_view_matrix;
		GLint uniform_perspective;

		// ͨ�� TutorialInterface �̳�
		virtual void initUniform() override;
	};
}


