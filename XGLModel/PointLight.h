#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class PointLight : public TutorialInterface
	{
	public:
		PointLight();
		virtual ~PointLight();

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

		GLint specular_Intensity;
		GLint shineness;
		GLint pointlight_eposition;
		GLint pointlight_attenuation_constant;
		GLint pointlight_attenuation_linear;
		GLint pointlight_attenuation_exp;
		GLint pointlight_light_color;
		GLint pointlight_light_ambient_intensity;
		GLint pointlight_light_diffuse_intensity;

	};
}

