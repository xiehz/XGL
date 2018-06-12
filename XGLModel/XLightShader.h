#pragma once
#include "GL\glew.h"
#include "xgl\Vec3f"
#include "XGLModel.h"
#include "XLight.h"
namespace XGLModel {

	class XLightShader
	{
	public:
		XLightShader();
		~XLightShader();
	public:

		void XGLModel::XLightShader::initUniform(GLuint program);
		void updateUniform(const SpotLighter& spl, float specular, float shineness);

	private:
		SpotLighter m_SpotLight;

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

