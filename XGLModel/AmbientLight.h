#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class AmbientLight : public TutorialInterface
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

		// ͨ�� TutorialInterface �̳�
		virtual void initGL() override;
		virtual void draw() override;

	private:
		GLuint textureObj;
		GLuint textureUnit;
		GLuint textureIndex;

		GLint u_mvp;
		GLint u_sampler;
		GLint u_ambient_ambient;
		GLint u_ambient_density;

		// ͨ�� TutorialInterface �̳�
		virtual void initUniform() override;
	};
}
