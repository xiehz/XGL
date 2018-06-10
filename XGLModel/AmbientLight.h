#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class AmbientLight : public TutorialInterface
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

		// 通过 TutorialInterface 继承
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

		// 通过 TutorialInterface 继承
		virtual void initUniform() override;
	};
}
