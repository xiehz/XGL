#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class AmbientLight : public TutorialInterface
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	private:
		GLuint textureObj;
		GLuint textureUnit;
		GLuint textureIndex;

		GLint u_mvp;
		GLint u_sampler;
		GLint u_ambient;
	};
}
