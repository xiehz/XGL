#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

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

		GLint specular_Intensity;
		GLint shineness;
		GLint spotlight_direction;
		GLint spotlight_cutoff;
		GLint spotlight_eposition;
		GLint spotlight_attenuation_constant;
		GLint spotlight_attenuation_linear;
		GLint spotlight_attenuation_exp;
		GLint spotlight_light_color;
		GLint spotlight_light_ambient_intensity;
		GLint spotlight_light_diffuse_intensity;
	};
}


