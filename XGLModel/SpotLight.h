#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"
#include "XLight.h"
#include "XLightShader.h"
namespace XGLModel {
	class SpotLight : public TutorialInterface
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

	private:
		GLuint texObj;
		GLuint texUnit;
		GLuint texUnitIndex;

		GLint g_mv;
		GLint g_perspective;
		GLint g_sampler;

		SpotLighter spotlight;
		XLightShader lightShader;
	};
}


