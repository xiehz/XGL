#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	
	class Tutorial6 : public TutorialInterface
	{
	public:
		Tutorial6();
		virtual ~Tutorial6();

		// 通过 TutorialInterface 继承
		virtual void initGL() override;
		virtual void draw() override;

	private:
		GLint uniform_mvp;
		GLint uniform_sampler;
		GLuint textureObj;
		GLuint textureUnit;
		GLuint textureUnitIndex;


		// 通过 TutorialInterface 继承
		virtual void initUniform() override;

	};
}


