#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	
	class Tutorial6 : public TutorialInterface
	{
	public:
		Tutorial6();
		virtual ~Tutorial6();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;

	private:
		GLint uniform_mvp;
		GLint uniform_sampler;
		GLuint textureObj;
		GLuint textureUnit;
		GLuint textureUnitIndex;


		// ͨ�� TutorialInterface �̳�
		virtual void initUniform() override;

	};
}


