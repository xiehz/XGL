#pragma once
#include "TutorialFactory.h"

namespace XGLModel {
	
	class Tutorial6 : public TutorialInterface
	{
	public:
		Tutorial6();
		virtual ~Tutorial6();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	private:
		GLint uniform_mvp;
		GLint uniform_sampler;
		GLuint textureObj;
		GLuint textureUnit;
		GLuint textureUnitIndex;

	};
}


