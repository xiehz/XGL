#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	class Tutorial5 :
		public TutorialInterface
	{
	public:
		Tutorial5();
		virtual ~Tutorial5();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	private:
		GLint uniform_scale;
		GLint uniform_model_matrix;
	};
}


